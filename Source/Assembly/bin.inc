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
    db
rom_bank_counter:
    db

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION "Video1", ROMX[$4000], BANK[1]
Video1:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank1.bin"

SECTION "Video2", ROMX[$4000], BANK[2]
Video2:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank2.bin"

SECTION "Video3", ROMX[$4000], BANK[3]
Video3:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank3.bin"

SECTION "Video4", ROMX[$4000], BANK[4]
Video4:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank4.bin"

SECTION "Video5", ROMX[$4000], BANK[5]
Video5:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank5.bin"

SECTION "Video6", ROMX[$4000], BANK[6]
Video6:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank6.bin"

SECTION "Video7", ROMX[$4000], BANK[7]
Video7:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank7.bin"

SECTION "Video8", ROMX[$4000], BANK[8]
Video8:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank8.bin"

SECTION "Video9", ROMX[$4000], BANK[9]
Video9:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank9.bin"

SECTION "Video10", ROMX[$4000], BANK[10]
Video10:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank10.bin"

SECTION "Video11", ROMX[$4000], BANK[11]
Video11:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank11.bin"

SECTION "Video12", ROMX[$4000], BANK[12]
Video12:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank12.bin"

SECTION "Video13", ROMX[$4000], BANK[13]
Video13:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank13.bin"

SECTION "Video14", ROMX[$4000], BANK[14]
Video14:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank14.bin"

SECTION "Video15", ROMX[$4000], BANK[15]
Video15:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank15.bin"

SECTION "Video16", ROMX[$4000], BANK[16]
Video16:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank16.bin"

SECTION "Video17", ROMX[$4000], BANK[17]
Video17:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank17.bin"

SECTION "Video18", ROMX[$4000], BANK[18]
Video18:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank18.bin"

SECTION "Video19", ROMX[$4000], BANK[19]
Video19:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank19.bin"

SECTION "Video20", ROMX[$4000], BANK[20]
Video20:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank20.bin"

SECTION "Video21", ROMX[$4000], BANK[21]
Video21:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank21.bin"

SECTION "Video22", ROMX[$4000], BANK[22]
Video22:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank22.bin"

SECTION "Video23", ROMX[$4000], BANK[23]
Video23:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank23.bin"

SECTION "Video24", ROMX[$4000], BANK[24]
Video24:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank24.bin"

SECTION "Video25", ROMX[$4000], BANK[25]
Video25:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank25.bin"

SECTION "Video26", ROMX[$4000], BANK[26]
Video26:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank26.bin"

SECTION "Video27", ROMX[$4000], BANK[27]
Video27:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank27.bin"

SECTION "Video28", ROMX[$4000], BANK[28]
Video28:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank28.bin"

SECTION "Video29", ROMX[$4000], BANK[29]
Video29:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank29.bin"

SECTION "Video30", ROMX[$4000], BANK[30]
Video30:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank30.bin"

SECTION "Video31", ROMX[$4000], BANK[31]
Video31:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank31.bin"

SECTION "Video32", ROMX[$4000], BANK[32]
Video32:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank32.bin"

SECTION "Video33", ROMX[$4000], BANK[33]
Video33:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank33.bin"

SECTION "Video34", ROMX[$4000], BANK[34]
Video34:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank34.bin"

SECTION "Video35", ROMX[$4000], BANK[35]
Video35:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank35.bin"

SECTION "Video36", ROMX[$4000], BANK[36]
Video36:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank36.bin"

SECTION "Video37", ROMX[$4000], BANK[37]
Video37:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank37.bin"

SECTION "Video38", ROMX[$4000], BANK[38]
Video38:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank38.bin"

SECTION "Video39", ROMX[$4000], BANK[39]
Video39:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank39.bin"

SECTION "Video40", ROMX[$4000], BANK[40]
Video40:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank40.bin"

SECTION "Video41", ROMX[$4000], BANK[41]
Video41:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank41.bin"

SECTION "Video42", ROMX[$4000], BANK[42]
Video42:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank42.bin"

SECTION "Video43", ROMX[$4000], BANK[43]
Video43:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank43.bin"

SECTION "Video44", ROMX[$4000], BANK[44]
Video44:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank44.bin"

SECTION "Video45", ROMX[$4000], BANK[45]
Video45:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank45.bin"

SECTION "Video46", ROMX[$4000], BANK[46]
Video46:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank46.bin"

SECTION "Video47", ROMX[$4000], BANK[47]
Video47:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank47.bin"

SECTION "Video48", ROMX[$4000], BANK[48]
Video48:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank48.bin"

SECTION "Video49", ROMX[$4000], BANK[49]
Video49:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank49.bin"

SECTION "Video50", ROMX[$4000], BANK[50]
Video50:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank50.bin"

SECTION "Video51", ROMX[$4000], BANK[51]
Video51:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank51.bin"

SECTION "Video52", ROMX[$4000], BANK[52]
Video52:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank52.bin"

SECTION "Video53", ROMX[$4000], BANK[53]
Video53:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank53.bin"

SECTION "Video54", ROMX[$4000], BANK[54]
Video54:
    INCBIN "../Bin/bad-apple-30fps-full/rom-bank54.bin"
