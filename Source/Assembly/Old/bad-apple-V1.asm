INCLUDE "hardware.inc"

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

.MainVideo1
    ld hl, Video1
    ld a, 1
    ld [$2000], a
    call Main_loop
.MainVideo2
    ld hl, Video2
    ld a, 2
    ld [$2000], a
    call Main_loop
.MainVideo3
    ld hl, Video3
    ld a, 3
    ld [$2000], a
    call Main_loop
.MainVideo4
    ld hl, Video4
    ld a, 4
    ld [$2000], a
    call Main_loop
.MainVideo5
    ld hl, Video5
    ld a, 5
    ld [$2000], a
    call Main_loop
.MainVideo6
    ld hl, Video6
    ld a, 6
    ld [$2000], a
    call Main_loop
.MainVideo7
    ld hl, Video7
    ld a, 7
    ld [$2000], a
    call Main_loop
.MainVideo8
    ld hl, Video8
    ld a, 8
    ld [$2000], a
    call Main_loop
.MainVideo9
    ld hl, Video9
    ld a, 9
    ld [$2000], a
    call Main_loop
.MainVideo10
    ld hl, Video10
    ld a, 10
    ld [$2000], a
    call Main_loop
.MainVideo11
    ld hl, Video11
    ld a, 11
    ld [$2000], a
    call Main_loop
.MainVideo12
    ld hl, Video12
    ld a, 12
    ld [$2000], a
    call Main_loop
.MainVideo13
    ld hl, Video13
    ld a, 13
    ld [$2000], a
    call Main_loop
.MainVideo14
    ld hl, Video14
    ld a, 14
    ld [$2000], a
    call Main_loop
.MainVideo15
    ld hl, Video15
    ld a, 15
    ld [$2000], a
    call Main_loop
.MainVideo16
    ld hl, Video16
    ld a, 16
    ld [$2000], a
    call Main_loop
.MainVideo17
    ld hl, Video17
    ld a, 17
    ld [$2000], a
    call Main_loop
.MainVideo18
    ld hl, Video18
    ld a, 18
    ld [$2000], a
    call Main_loop
.MainVideo19
    ld hl, Video19
    ld a, 19
    ld [$2000], a
    call Main_loop
.MainVideo20
    ld hl, Video20
    ld a, 20
    ld [$2000], a
    call Main_loop
.MainVideo21
    ld hl, Video21
    ld a, 21
    ld [$2000], a
    call Main_loop
.MainVideo22
    ld hl, Video22
    ld a, 22
    ld [$2000], a
    call Main_loop
.MainVideo23
    ld hl, Video23
    ld a, 23
    ld [$2000], a
    call Main_loop
.MainVideo24
    ld hl, Video24
    ld a, 24
    ld [$2000], a
    call Main_loop
.MainVideo25
    ld hl, Video25
    ld a, 25
    ld [$2000], a
    call Main_loop
.MainDone
    jp .MainDone
    




Main_loop:
    ld e, 5

    ld a, [vblank_counter]
    ld b, a

    call WaitVBlank2

    ld a, b
    ld [vblank_counter], a

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
    cp 80
    jp nc, Mode1
    jp Mode2
    ; Choose mode


EndFrame:
    ld a, [vblank_counter]
    ld b, a
.EndFrameloop:
    ld a, b
    cp 6
    jp nc, .EndFramedone
    call WaitVBlank2
    jp .EndFrameloop
.EndFramedone:
    ld a, 0
    ld [vblank_counter], a
    jp Main_loop
    
EndRom:
    ld a, 0
    ld [vblank_counter], a
    ret



Mode1:
    sub a, 80
    ld d, a

.Mode1loop:
    ld a, [hli]
    ld [bc], a
    ; load tile ID to ppu adress

    inc bc
    dec d
    jp nz, .Mode1loop

    dec e
    jp z, Main_loop
    ; check if 5 packets have been decoded

    jp FetchPacket


Mode2: 
    ld d, a
    ld a, [hli]

.Mode2loop:
    ld [bc], a
    ; load tile ID to ppu adress

    inc bc
    dec d
    jp nz, .Mode2loop

    dec e
    jp z, Main_loop
    ; check if 5 packets have been decoded

    jp FetchPacket


; Functions

WaitVBlank1:
    ld a, [rLY]
    cp 144
    jp nz, WaitVBlank1
    ret

WaitVBlank2:
    inc b
.WaitNotVblank:
    ld a, [rLY]
    cp 144
    jp nc, .WaitNotVblank
.WaitVBlank2loop:
    ld a, [rLY]
    cp 144
    jp nz, .WaitVBlank2loop
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







; Binaries:

SECTION "Tileset", ROM0

Tileset:
    INCBIN "tileset.bin"
TilesetEnd:

Tilemap:
    INCBIN "frame1.bin"
TilemapEnd:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION "Variables", WRAM0

vblank_counter:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION "Video1", ROMX[$4000], BANK[1]

Video1:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank1.bin"
Video1End:

SECTION "Video2", ROMX[$4000], BANK[2]

Video2:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank2.bin"
Video2End:

SECTION "Video3", ROMX[$4000], BANK[3]

Video3:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank3.bin"
Video3End:

SECTION "Video4", ROMX[$4000], BANK[4]

Video4:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank4.bin"
Video4End:

SECTION "Video5", ROMX[$4000], BANK[5]

Video5:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank5.bin"
Video5End:

SECTION "Video6", ROMX[$4000], BANK[6]

Video6:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank6.bin"
Video6End:

SECTION "Video7", ROMX[$4000], BANK[7]

Video7:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank7.bin"
Video7End:

SECTION "Video8", ROMX[$4000], BANK[8]

Video8:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank8.bin"
Video8End:

SECTION "Video9", ROMX[$4000], BANK[9]

Video9:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank9.bin"
Video9End:

SECTION "Video10", ROMX[$4000], BANK[10]

Video10:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank10.bin"
Video10End:

SECTION "Video11", ROMX[$4000], BANK[11]

Video11:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank11.bin"
Video11End:

SECTION "Video12", ROMX[$4000], BANK[12]

Video12:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank12.bin"
Video12End:

SECTION "Video13", ROMX[$4000], BANK[13]

Video13:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank13.bin"
Video13End:

SECTION "Video14", ROMX[$4000], BANK[14]

Video14:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank14.bin"
Video14End:

SECTION "Video15", ROMX[$4000], BANK[15]

Video15:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank15.bin"
Video15End:

SECTION "Video16", ROMX[$4000], BANK[16]

Video16:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank16.bin"
Video16End:

SECTION "Video17", ROMX[$4000], BANK[17]

Video17:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank17.bin"
Video17End:

SECTION "Video18", ROMX[$4000], BANK[18]

Video18:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank18.bin"
Video18End:

SECTION "Video19", ROMX[$4000], BANK[19]

Video19:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank19.bin"
Video19End:

SECTION "Video20", ROMX[$4000], BANK[20]

Video20:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank20.bin"
Video20End:

SECTION "Video21", ROMX[$4000], BANK[21]

Video21:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank21.bin"
Video21End:

SECTION "Video22", ROMX[$4000], BANK[22]

Video22:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank22.bin"
Video22End:

SECTION "Video23", ROMX[$4000], BANK[23]

Video23:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank23.bin"
Video23End:

SECTION "Video24", ROMX[$4000], BANK[24]

Video24:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank24.bin"
Video24End:

SECTION "Video25", ROMX[$4000], BANK[25]

Video25:
INCBIN "../Bin/bad-apple-10fps-full/rom-bank25.bin"
Video25End: