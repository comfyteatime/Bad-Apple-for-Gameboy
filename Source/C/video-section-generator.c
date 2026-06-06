// generates the include statements for RGBASM to include the video binaries

#include <stdio.h>

void main(){
    for(int i=1; i<=54; i++){
        printf("SECTION %cVideo%d%c, ROMX[$4000], BANK[%d]\n", '"', i, '"', i);
        printf("Video%d:\n", i);
        printf("    INCBIN %c../Bin/bad-apple-30fps-full/rom-bank%d.bin%c\n", '"', i, '"');
        printf("\n");
    }
}