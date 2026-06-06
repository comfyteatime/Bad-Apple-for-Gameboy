/*    data for a single tile in our set. Each line represents a row of pixels.
    the leftmost digit represents the leftmost pixel of the row
    each pixel has a value from 0-3

Each tile consists of 16 bytes. Each row is stored as two bytes.
Unintuitively however, the data for each pixel of a tile is split among the two bytes.
Say we have the tile: 01 10 11 00   11 01 00 10
the value of the leftmost pixel is actually 10. 
This is because it takes the most significant digit from both bytes,
with the first digit coming from the second byte (the 1) and the second digit coming from the first byte (the 0)
So the bytes (01 10 11 00   11 01 00 10), correspond to the pixel values, from left to right (10 11 01 10 01 01 10 00)

So the first byte stores the least significant bit of every pixel, and the second byte the most significant bit.
*/

//This program generates a tile set of all possible tiles consisting of 4 equal 4x4 2bpp squares (4^4 = 256 tiles total)


//apparently, the 256th tile is always white no matter what, so we generate our map to match the last tile with white.

#include <stdio.h>
#include <stdint.h>


unsigned int tileset[256][4];
int counter = 0;

uint8_t rowBuffer[2]; //rowBuffer[0] stores current tile row's least significant bits | rowBuffer[1] stores current tile row's most significant bits


int main(){
    //generate every possible tile
    FILE *pF = fopen("./tileset.bin","wb");
    if(pF == NULL) {
        printf("Couldn't open tileset.bin. Aborting...\n");
        return 1;
    }

    for(int i=0; i<4; i++) for(int j=0; j<4; j++) for(int k=0; k<4; k++) for(int h=0; h<4; h++) {
        tileset[counter][0] = 3-i;
        tileset[counter][1] = 3-j;
        tileset[counter][2] = 3-k;
        tileset[counter][3] = 3-h;
        counter++;
    }
    //convert to gameboy tile format
    for(int i=0; i<256; i++){
        for(int j=0; j<8; j++){
            int x = j/4;
            // 240 = 2^7 + 2^6 + 2^5 + 2^4 (represents the 4 leftmost bits) | 15 = 2^3 + 2^2 + 2^1 + 2^0 (represents the 4 rightmost bits)
            rowBuffer[0] = 240*(tileset[i][2*x] & 1) + 15*(tileset[i][1+2*x] & 1);     
            rowBuffer[1] = 240*(tileset[i][2*x] & 2)/2 + 15*(tileset[i][1+2*x] & 2)/2;
            fwrite(rowBuffer, sizeof(uint8_t), 2, pF);
        }
    }
    printf("Succesfully generated tileset.bin\n");
    fclose(pF);
    return 0;
}