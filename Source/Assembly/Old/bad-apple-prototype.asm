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
    ld a, LCDCF_ON | LCDCF_BGON | LCDCF_BG8000
    ld [rLCDC], a

    ; Turns on the lcd.
    ; The LCDCF constants are defined in hardware.inc, and represent the LCDC registers bits values.

    ld a, %11100100 ; ID3: 11 | ID2: 10 | ID1: 01 | ID0: 00
    ld [rBGP], a

    ; sets backgroud palette

    ld hl, Video

    ld bc, 0
    push bc
    ; vblank counter for frame
    

Main_loop:
    ld e, 5
    pop bc
    call WaitVBlank2
    push bc
FetchPacket:
    ld a, [hli]
    cp $FF
    jp z, EndFrame
    ; Check if end of frame data has been reached ($FF = End of frame)

    cp $EE
    jp z, Done
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
    pop bc
.EndFrameloop:
    ld a, c
    cp 6
    jp nc, .EndFramedone
    call WaitVBlank2
    jp .EndFrameloop
.EndFramedone:
    ld bc, 0
    push bc
    jp Main_loop
    


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


Done:
    jp EntryPoint ; originally done, but now entrypoint to repeat




; Functions

WaitVBlank1:
    ld a, [rLY]
    cp 144
    jp nz, WaitVBlank1
    ret

WaitVBlank2:
    inc bc
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

SECTION "Video", ROM0[$4000]

Video:
    INCBIN "../Bin/bad-apple-10fps-full/rom-bank3.bin"
VideoEnd: