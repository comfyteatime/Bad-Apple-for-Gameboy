// This version generates tilemaps without accounting for the full 32x32 map (not filling offscreen tiles with id 0)
// These tilemaps can be used as inputs for the video encoder

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TILEWIDTH  20
#define TILEHEIGHT  18
#define FRAMESIZE  TILEWIDTH*TILEHEIGHT
#define FULLTILEMAPLENGTH 32 // Gameboy tilemap is actually 32x32, but only a 20x18 area is visible

#define IMAGEWIDTH 160
#define IMAGEHEIGHT 144
#define IMAGE_SIZE IMAGEWIDTH * IMAGEHEIGHT
#define MAGICNUMBER "P5"

const uint8_t empty_tile = 0;

uint8_t tileset[256][64];
uint8_t tileset_buffer[2];

int image[IMAGE_SIZE];
int image_tile_region[64];
int image_buffer;

int best_score;
int current_score;
uint8_t best_fit_tile_index;

uint8_t read_byte_length;

//metadata
char magic_number[3];
int image_width;
int image_height;
int maxval;


void load_tileset(FILE *file){
    for(int i=0; i<256; i++)  for(int j=0; j<8; j++){
        fread(tileset_buffer, sizeof(uint8_t), 2, file);
        for(int k=0; k<8; k++){
            tileset[i][8*j+k] = ((tileset_buffer[0] >> (7 - k)) & 1) + (((tileset_buffer[1] >> (7 - k)) & 1) << 1);
        }
    }
}

int load_metadata(FILE *file){
    if(fscanf(file, "%2s %d %d %d", magic_number, &image_width, &image_height, &maxval) != 4) return 1;
    printf("Metadata: %s %d %d %d\n", magic_number, image_width, image_height, maxval);

    if((strcmp(magic_number, MAGICNUMBER)!=0) || (image_width!=IMAGEWIDTH) || (image_height!=IMAGEHEIGHT)) return 1;
    if((0<maxval) && (maxval<256)) read_byte_length = 1;
    else if((256<=maxval) && (maxval<65536)) read_byte_length = 2;
    else return 1;
    return 0;
}

void load_tile_region(int row, int collumn){
    int counter = 0;
    for(int i=0; i<8; i++) for(int j=0; j<8; j++){
        image_tile_region[counter] = image[i*IMAGEWIDTH + j + 8*IMAGEWIDTH*row + 8*collumn];
        counter += 1;
    }

}

int tile_score(int tileset_index){
    int score = 0;
    for(int i=0; i<64; i++){
        int converted_pixel = 4 *(maxval-image_tile_region[i]) / (maxval+1);
        score += abs(converted_pixel - tileset[tileset_index][i]);
    }
    return score;
}


int main(int argc, char *argv[]){
    if(argc != 4){
        printf("Wrong argument count, pass arguments like so: \n./tilemap-generator.sh [input tileset (.bin)] [source image (.pgm)] [output tilemap (.bin)]\n");
        return 1;
    }
    
    FILE *FTileset = fopen(argv[1], "rb");
    if(FTileset == NULL){
        printf("Error, %s not found. Aborting...\n", argv[1]);
        return 1;
    }

    load_tileset(FTileset);

    FILE *FImage = fopen(argv[2], "rb");
    if(FImage == NULL){
        printf("Error, %s not found. Aborting...\n", argv[2]);
        fclose(FTileset);
        return 1;
    }

    if(load_metadata(FImage) == 1){
        printf("Error, metadata of %s incorrect. Aborting...\n", argv[2]);
        fclose(FTileset);
        fclose(FImage);
        return 1;
    }

    for(int i=0; i<IMAGE_SIZE; i++){
        fread(&image_buffer, read_byte_length, 1, FImage);
        image[i] = image_buffer;
    }

    FILE *FTilemap = fopen(argv[3], "wb");
    if(FImage == NULL){
        printf("Error, failed to open %s. Aborting...\n", argv[3]);
        fclose(FTileset);
        fclose(FImage);
        return 1;
    }
    printf("Tilemap: \n");
    for(int row=0; row<TILEHEIGHT; row++) {
        for(int column=0; column<TILEWIDTH; column++) {
            best_score = 99999;
            load_tile_region(row, column);
            for(int k=0; k<256; k++){
                current_score = tile_score(k);
                if(current_score < best_score){
                    best_fit_tile_index = k;
                    best_score = current_score;
                }
            }
            if(best_fit_tile_index<16) printf("0"); // adds 0 to singular digit hex numbers, to make all tile IDs 2 digits
            printf("%x ", best_fit_tile_index);

            fwrite(&best_fit_tile_index, sizeof(uint8_t), 1, FTilemap);
        }
        printf("\n");
    }

    printf("Succesfully generated %s.\n", argv[3]);
    fclose(FTileset);
    fclose(FImage);
    fclose(FTilemap);
    return 0;
}