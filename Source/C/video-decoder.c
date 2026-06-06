// Decodes a given bin video file, and outputs the tilemap for every frame in the terminal
// Useful for checking whether the video encoder properly encoded everything

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define TILEWIDTH  20
#define TILEHEIGHT  18
#define FRAMESIZE  TILEWIDTH*TILEHEIGHT
#define FULLTILEMAPLENGTH 32 // Gameboy tilemap is actually 32x32, but only a 20x18 area is visible
#define FULLTILEMAPSIZE FULLTILEMAPLENGTH*FULLTILEMAPLENGTH

const int TILEMAP_START = 0x9800;
const int TILEMAP_END = 0x9BFF;
const int FRAME_END = 0xFF;
const int ROM_END = 0xEE;
const int MODE1_IDENTIFIER = 80;

uint8_t frame[FULLTILEMAPSIZE];

uint16_t packet_adress;
uint8_t packet_length;

int frame_counter = 0;

uint8_t buffer;

FILE *pROM;
FILE *pFRAME;

void print_frame(){
    frame_counter += 1;
    printf("Frame %d\n", frame_counter);
    for(int i=0; i<TILEHEIGHT; i++){
        for(int j=0; j<TILEWIDTH; j++){
            if(frame[i*FULLTILEMAPLENGTH + j] < 16) printf("0"); // adds 0 to singular digit hex numbers, to make all tile IDs 2 digits
            printf("%x ", frame[i*FULLTILEMAPLENGTH + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void mode_1(){
    packet_length -= MODE1_IDENTIFIER;
    packet_adress -= TILEMAP_START; // not accounting for the full tile map size 32x32
    for(int i=0; i<packet_length; i++){
        fread(&buffer, sizeof(uint8_t), 1, pROM);
        frame[packet_adress] = buffer;
        packet_adress += 1;
    }
}

void mode_2(){
    packet_adress -= TILEMAP_START;
    fread(&buffer, sizeof(uint8_t), 1, pROM);
    for(int i=0; i<packet_length; i++){
        frame[packet_adress] = buffer;
        packet_adress += 1;
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Wrong argument count, pass arguments like so: \n./video-decoder.sh [input video (.bin)]\n");
        return 1;
    }

    for(int i=0; i<FULLTILEMAPSIZE; i++) frame[i] = 0x00;
    pROM = fopen(argv[1], "rb");
    if(pROM == NULL) {
        printf("Error, %s not found. Aborting...\n", argv[1]);
        return 1;
    }

    while(true){
        fread(&buffer, sizeof(uint8_t), 1, pROM);
        if(buffer == FRAME_END) {
            print_frame();
            continue;
        }
        else if(buffer == ROM_END) {
            break;
        }
        packet_adress = buffer << 8; // shift most significant adres bytes to the left
        fread(&buffer, sizeof(uint8_t), 1, pROM);
        packet_adress += buffer;

        if((packet_adress < TILEMAP_START) || (packet_adress > TILEMAP_END)){
            printf("Error, package adress is outside of tilemap range (0x%x - 0x%x). Aborting.\n", TILEMAP_START, TILEMAP_END);
            fclose(pROM);
            return 1;
        }

        fread(&packet_length, sizeof(uint8_t), 1, pROM);
        if(packet_length>MODE1_IDENTIFIER) mode_1();
        else mode_2();
    }
    fclose(pROM);
    return 0;
}