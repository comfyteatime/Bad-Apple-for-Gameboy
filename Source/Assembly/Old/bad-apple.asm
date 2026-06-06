INCLUDE "hardware.inc"

; "hardware.inc" has a bunch of useful constants for registers, register states, etc.

SECTION "header", ROM0[$100]

    jp EntryPoint
    ; after displaying the logo, the built in boot rom jumps the instruction adress to $100
    ; so we need a jump to actually go to our program code, so that we don't start executing the header data as if they were instructions

    ds $150 - @, 0
    ; @ = adress of the instruction it's used in (current instruction adress)
    ; since the header goes from $100 to $150, we use ds (define space) to free space for the header 
    ; 0 is the value to write to all the bytes in this space

EntryPoint:  
; This is a label, which is just a name given to an adress in the rom.
; It corresponds to the adress of the first instruction after the label.
; Since this comes just after the header, the adress of EntryPoint is $151.

WaitVBlank:
    ld a, [rLY]
    cp 144
    jp c, WaitVBlank

    ; When the lcd is done drawing pixels to the screen, it enters VBlank mode.
    ; This happens when the register LY (LCD Y) reaches 144, which is after the 144 lines of pixels on the lcd (160x144 resolution).
    ; During this mode you can safely turn off the lcd, since it is not being drawn to. 
    ; Doing so outside of VBlank could damage the screen of a real gameboy, so be careful.

    ; Turn off lcd
    ld a, 0
    ld [rLCDC], a

    ; Load 0 to LCDC to turn off lcd and all lcd toggles.

CopyTileset:
    ld de, Tileset
    ld hl, $8000
    ld bc, TilesetEnd - Tileset
    call Memcopy

    ; The tileset data is stored in 3 blocks: Block 0: $8000-87FF (0-127)| Block 1: $8800–8FFF (128-255)| Block 2: $9000–97FF (0-127).
    ; Depending on the value of the 4th bit of LCDC, we use either bank 0 (bit = 1) or bank 2 (bit = 0).
    ; So we load our set from blocks 0 and 1, since our tileset is a single bin file and banks 0 and 1 are consecutive.


CopyTilemap:
    ld de, Tilemap
    ld hl, $9800
    ld bc, TilemapEnd - Tilemap
    call Memcopy

    ; The tilemap lies in range $9800-9FFF in VRAM

    ld a, LCDCF_ON | LCDCF_BGON | LCDCF_BG8000
    ld [rLCDC], a

    ; Turns on the lcd.
    ; The LCDCF constants are defined in hardware.inc, and represent the LCDC registers bits values.

    ld a, %11100100 ; ID3: 11 | ID2: 10 | ID1: 01 | ID0: 00
    ld [rBGP], a

    ; Initialize display registers during first blank frame
    ; Sets the background palette (BGP = background pallete).
    ; The first two bits are ID 3, the next two ID 2, and so on.

Done:
    jp Done


    /* 
    LCDC is the LCD Control register. Each bit represents a different toggle for the lcd:
    LCD & PPU enable: 0 = Off; 1 = On
    Window tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF
    Window enable: 0 = Off; 1 = On
    BG & Window tile data area: 0 = 8800–97FF; 1 = 8000–8FFF
    BG tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF
    OBJ size: 0 = 8×8; 1 = 8×16
    OBJ enable: 0 = Off; 1 = On
    BG & Window enable / priority [Different meaning in CGB Mode]: 0 = Off; 1 = On
    */


Memcopy:
    ld a, [de]
    ld [hli], a
    inc de
    dec bc
    ld a, b
    or a, c
    jp nz, Memcopy
    ret

    ; We can make functions in assembly using labels.
    ; Place the label of the function after the main program loop so that it won't be executed unless called upon.
    ; Then to call the function, use the call instruction to push the current instruction adress to the stack and jump to the function.
    ; At the end of the function, add a ret instruction, which will pop the last instruction adress which was called from off the stack and jump back to it.


SECTION "Tileset", ROM0

Tileset:
    INCBIN "tileset.bin"
TilesetEnd:


SECTION "Tilemap", ROM0

Tilemap:
    INCBIN "tilemap.bin"
TilemapEnd:

; INCBIN stores the binary data from a file in the ROM starting from the instructions adress.