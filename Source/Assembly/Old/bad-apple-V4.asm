INCLUDE "hardware.inc"
INCLUDE "bin.inc"

; Constants

def ROM_BANK_START equ $4000
def START_VBLANK equ 144
def MODE1_IDENTIFIER equ 80
def NUM_ROMBANKS equ 25
def PPU_CYCLES_PER_FRAME equ 3

SECTION "Header", ROM0[$100]

    jp EntryPoint
    ds $150 - @, 0

EntryPoint:
    call WaitVBlank1
    ld a, 0
    ld [rLCDC], a
    ; turn off lcd

CopyTileset:
    ld de, Tileset
    ld hl, $8000
    ld bc, TilesetEnd - Tileset
    call Memcopy

CopyTilemap:
    ld de, Tilemap
    ld hl, $9800
    ld bc, TilemapEnd - Tilemap
    call Memcopy

Main:
.MainSetup
    ld a, LCDCF_ON | LCDCF_BGON | LCDCF_BG8000
    ld [rLCDC], a
    ; Turns on the lcd.
    ; The LCDCF constants are defined in hardware.inc, and represent the LCDC registers bits values.

    ld a, %11100100 ; ID3: 11 | ID2: 10 | ID1: 01 | ID0: 00
    ld [rBGP], a
    ; sets backgroud palette

    ld a, 0
    ld [vblank_counter], a
    ld [rom_bank_counter], a
.Main_loop
    call SwitchRomBank
    call Write_loop
    cp NUM_ROMBANKS
    jp c, .Main_loop
.MainLCD_off
    call WaitVBlank2
    ld a, 0
    ld [rLCDC], a
    ; turn off lcd
.MainDone
    jp .MainDone


; Functions

Write_loop:
    call WaitUntilNotVblank
FetchPacket:
    ld a, [hli]
    cp $FF
    jp z, EndFrame
    ; Check if end of frame data has been reached ($FF = End of frame)

    cp $EE
    jp z, EndRom
    ; Check if end of rom has been reached ($EE = End of rom)

    ld b, a
    ld a, [hli]
    ld c, a 
    ; load ppu adress to bc

    ld a, [hli]
    cp MODE1_IDENTIFIER
    jp nc, Mode1
    jp Mode2
    ; Choose mode

EndFrame:
    ld a, [vblank_counter]
    ld b, a
.EndFrameloop
    ld a, b
    cp PPU_CYCLES_PER_FRAME
    jp nc, .EndFramedone
    call WaitVBlank2
    jp .EndFrameloop
.EndFramedone
    ld a, 0
    ld [vblank_counter], a
    jp Write_loop
    

Mode1:
    sub a, MODE1_IDENTIFIER
    ld d, a
.Mode1loop
    call WaitH_or_Vblank
    ld a, [hli]
    ld [bc], a
    ; load tile ID to ppu adress

    inc bc
    dec d
    jp nz, .Mode1loop

    ld a, [rLY]
    cp 152
    jp nc, Write_loop
    ; once there are only one or two scanlines of vblank left, go to Write_loop

    jp FetchPacket


Mode2: 
    ld d, a
    ld a, [hli]
    ld e, a
.Mode2loop
    call WaitH_or_Vblank
    ld a, e
    ld [bc], a
    ; load tile ID to ppu adress

    inc bc
    dec d
    jp nz, .Mode2loop

    jp FetchPacket


WaitVBlank1:
    ld a, [rLY]
    cp START_VBLANK
    jp nz, WaitVBlank1
    ret

WaitVBlank2:
    inc b
.WaitNotVblank
    ld a, [rLY]
    cp START_VBLANK
    jp nc, .WaitNotVblank
.WaitVBlank2loop
    ld a, [rLY]
    cp START_VBLANK
    jp nz, .WaitVBlank2loop
    ret


WaitH_or_Vblank:
.wait
    ld a, [rSTAT]
    and a, %00000011 ; check last two bits for ppu mode
    jp z, .wait
    cp 1
    jp z, .IsVblank
    jp .WaitHblank
.IsVblank
    ld a, [rLY]
    cp 152
    jp nc, .WaitNextFrame
    ret
.WaitNextFrame
    ld a, [vblank_counter]
    inc a
    ld [vblank_counter], a
.WaitLY0
    ld a, [rLY]
    cp 152
    jp nc, .WaitLY0
    ret
.WaitHblank
    ld a, [rSTAT]
    and a, %00000011
    jp nz, .WaitHblank
    ret

WaitUntilNotVblank:
    ld a, [rLY]
    cp START_VBLANK
    jp nc, WaitUntilNotVblank
    ret


EndRom:
    ld a, 0
    ld [vblank_counter], a
    ret

Memcopy:  ; (hl = destination, de = source, bc = num of tiles to copy)
    ld a, [de]
    ld [hli], a
    inc de
    dec bc
    ld a, b
    or a, c
    jp nz, Memcopy
    ret

SwitchRomBank:
    ld a, [rom_bank_counter]
    inc a
    ld [$2000], a
    ld hl, ROM_BANK_START
    ld [rom_bank_counter], a
    ret