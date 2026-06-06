INCLUDE "hardware.inc"

SECTION "header", ROM0[$100]

    jp EntryPoint
    ds $150 - @, 0

EntryPoint:
    call WaitVBlank
    ld a, 0
    ld [rLCDC], a
    ; Turn lcd off

CopyTileset:
    ld de, Tileset
    ld hl, $8000
    ld bc, TilesetEnd - Tileset
    call Memcopy

    ld a, LCDCF_ON | LCDCF_BGON | LCDCF_BG8000
    ld [rLCDC], a

    ld a, %11100100 ; ID3: 11 | ID2: 10 | ID1: 01 | ID0: 00
    ld [rBGP], a


Main:
    ld [hl], $4000 ; hl = load_adress
    call Main_loop
    ; ROM1

    ; Change rom bank to ROM2

    ld [hl], $4000 ; hl = tile ID load adress
    call Main_loop
    ; ROM2

    ld [hl], $4000 ; hl = load_adress
    call Main_loop
    ; ROM3

    ; Change rom bank to ROM3, and so on

    jp Done


Done:
    jp Done



; FUNCTIONS

WaitVBlank:
    ld a, [rLY]
    cp 144
    jp c, WaitVBlank
    ret

Memcopy:
    ld a, [de]
    ld [hli], a
    inc de
    dec bc
    ld a, b
    or a, c
    jp nz, Memcopy
    ret

Main_loop:
    call WaitVBlank
    ; for 10 fps, call 6 times i guess?
    
FetchPacket:
    ld a, [hli]
    cp $FF
    jp z Main_loop
    ; Check if end of frame data has been reached ($FF = End of frame)

    cp $EE
    jp z EndRom
    ; Check if end of rom has been reached ($EE = End of rom)

    ld b, a
    ld a, [hli]
    ld c, a 
    ; load ppu adress to bc

    ld a, [hli]
    ld d, a 
    ; load length to d
    cp 80
    jp nc Mode1
    jp Mode2
    ; Choose mode

EndRom:
    ret


Mode1:
    ld a, [hli]
    ld [bc],
    ; load tile ID to ppu adress

    inc bc
    dec d
    ld a, d
    cp 80
    jp nz Mode1
    jp FetchPacket

Mode2: 
    ld a, [hl]
    ld [bc],
    ; load tile ID to ppu adress

    inc bc
    dec d
    ld a, d
    cp 0
    jp nz Mode2
    jp FetchPacket



SECTION "Tileset", ROM0

Tileset:
    INCBIN "tileset.bin"
TilesetEnd:

SECTION "Music", ROM0
    ; Include music here



; ROM banks for frame data:

SECTION "Frames1", ROM1

Frames1:
    INCBIN "Frames1.bin"
Frames1End:

SECTION "Frames2", ROM2

Frames2:
    INCBIN "Frames2.bin"
Frames2End:
