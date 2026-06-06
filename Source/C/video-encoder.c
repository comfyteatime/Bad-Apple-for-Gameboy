// Encodes the tilemaps for each individual frame (frame%d.bin, starting from frame1.bin) into a series of 16kb bin files,
// 1 for each rom bank on the cartridge.

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define TILEWIDTH 20
#define TILEHEIGHT 18
#define FRAMESIZE TILEWIDTH*TILEHEIGHT
#define FULLTILEMAPLENGTH 32 // Gameboy tilemap is actually 32x32, but only a 20x18 area is visible
#define MAX_SUBROWLEN TILEWIDTH/2 

const int TILEMAP_START = 0x9800;
const int FRAME_END = 0xFF;
const int ROM_END = 0xEE;
const int NO_TILE = 0XFFFF;
const int MODE1_IDENTIFIER = 80;
const int METADATA = 3;
const int ROM_BANK_CAPACITY = 16000; //16 kilobytes per rom bank
const int MAX_FRAME_WRITE = 144 * 23 + 1 + 1; // 144 times the max row size (bigpacket, 20 tiles) + frame_end byte + rom_end byte

uint8_t previous_frame[FRAMESIZE];
uint8_t current_frame[FRAMESIZE];
uint16_t row_buffer[TILEWIDTH]; // 16 bit so that for none changed tiles we can use NO_TILE

// organize into typedef struct?
uint16_t subrows[MAX_SUBROWLEN][TILEWIDTH];
int num_subrows;
int subrow_lens[MAX_SUBROWLEN];
int subrow_start_collumn[MAX_SUBROWLEN];
int best_methods[MAX_SUBROWLEN];
int best_scores[MAX_SUBROWLEN];

FILE *pROM;
FILE *pFRAME;

int rom_counter = 0;
int frame_counter = 0;

int rom_bank_size;


void change_rom_bank(char *argv[]){
    rom_counter++;
    rom_bank_size = 0;
    char temp[100];
    sprintf(temp, "%s/rom-bank%d.bin", argv[2], rom_counter);
    if(pROM != NULL) fclose(pROM);
    pROM = fopen(temp, "wb");
    if(pROM == NULL){
        printf("Error, failed to open rom-bank%d.bin. Aborting...\n", rom_counter);
        exit(1);
    }
}

bool rom_bank_full(){
    if(ROM_BANK_CAPACITY - rom_bank_size < MAX_FRAME_WRITE) return true;
    return false;
}


void load_frame(char *argv[]){
    frame_counter++;
    char temp[100];
    sprintf(temp, "%s/frame%d.bin", argv[1], frame_counter);
    pFRAME = fopen(temp, "rb");
    if(pFRAME == NULL){
        printf("Error, failed to open frame%d.bin. Aborting...\n", frame_counter);
        if(pROM != NULL) fclose(pROM);
        exit(1);
    }
    fread(current_frame, sizeof(uint8_t), FRAMESIZE, pFRAME);
    fclose(pFRAME);
}

bool next_frame_null(char *argv[]){
    char temp1[100];
    sprintf(temp1, "%s/frame%d.bin", argv[1], frame_counter+1);
    FILE *temp2 = fopen(temp1, "rb");
    if(temp2 == NULL) return true;
    fclose(temp2);
    return false;
}

void set_previous_frame(){
    for(int i=0; i<FRAMESIZE; i++)  previous_frame[i] = current_frame[i];
}


void load_subrows(){
    num_subrows = 0;
    int counter = 0;
    int subrow_index = 0;
    while(counter<TILEWIDTH){
        if(row_buffer[counter] != NO_TILE){
            subrow_start_collumn[num_subrows] = counter;
            num_subrows++;
            subrow_index = 0;
            while(row_buffer[counter] != NO_TILE){
                subrows[num_subrows-1][subrow_index] = row_buffer[counter];
                subrow_index++;
                counter++;
                if (counter>=TILEWIDTH) break;
            }
            subrow_lens[num_subrows-1] = subrow_index;
        }
        counter++;
    } 
}

void evaluate_repetition_method(int subrow, int min_rep_len, int *current_score){
    int p1 = 0;
    int p2 = 0;
    int counter = 0;
    int rep_len = 0;
    while(counter<subrow_lens[subrow]-1){
        if(subrows[subrow][counter] == subrows[subrow][counter+1]){
            p2 = counter;
            while((subrows[subrow][counter] == subrows[subrow][counter+1]) && (counter<subrow_lens[subrow]-1)){
                counter++;
            }
            rep_len = counter - p2 + 1;
            if(rep_len>min_rep_len){
                if(p2-p1 > 0) *current_score += METADATA + (p2 - p1);
                *current_score += (METADATA + 1);
                p1 = counter + 1;
            }
        }
        else counter++;
    }
    counter = subrow_lens[subrow];
    if(p1 != counter) *current_score += METADATA + (counter-p1);
}


void test_subrow(int subrow){
    int current_score;

    // method 1
    current_score = METADATA + subrow_lens[subrow];
    best_scores[subrow] = current_score;
    best_methods[subrow] = 1;

    // method 2
    current_score = 0;
    for(int i=0; i<subrow_lens[subrow]-1; i++) if(subrows[subrow][i] != subrows[subrow][i+1]) current_score += 4;
    current_score += 4;
    if(current_score<best_scores[subrow]){
        best_scores[subrow] = current_score;
        best_methods[subrow] = 2;
    }

    // method 3
    current_score = 0;
    evaluate_repetition_method(subrow, 4, &current_score);
    if(current_score<best_scores[subrow]){
        best_scores[subrow] = current_score;
        best_methods[subrow] = 3;
    }

    // method 4  
    current_score = 0;
    evaluate_repetition_method(subrow, 7, &current_score);
    if(current_score<best_scores[subrow]){
        best_scores[subrow] = current_score;
        best_methods[subrow] = 4;
    }
}

// the term "methods" for packets changed to modes, because of term overlap

void write_mode1(uint16_t packet_adress, uint8_t packet_length, int row){ 
    uint8_t tile_buffer[TILEWIDTH];
    int temp_len = packet_length;
    int column = (packet_adress - TILEMAP_START) % FULLTILEMAPLENGTH;

    // use packet adress and length to find tile ID's and store them in buffer
    for(int i=0; i<packet_length; i++)  tile_buffer[i] = current_frame[column + row * TILEWIDTH + i];
    packet_adress = (packet_adress << 8) + (packet_adress >> 8); // convert to big endian
    packet_length += MODE1_IDENTIFIER;

    fwrite(&packet_adress, sizeof(uint16_t), 1, pROM);
    fwrite(&packet_length, sizeof(uint8_t), 1, pROM);    
    for(int i=0; i<temp_len; i++) fwrite(&tile_buffer[i], sizeof(uint8_t), 1, pROM);

    rom_bank_size += METADATA + temp_len;
}

void write_mode2(uint16_t packet_adress, uint8_t packet_length, int row){
    int column = (packet_adress - TILEMAP_START) % FULLTILEMAPLENGTH;
    uint8_t tileID = current_frame[column + row * TILEWIDTH];
    packet_adress = (packet_adress << 8) + (packet_adress >> 8); // convert to big endian

    fwrite(&packet_adress, sizeof(uint16_t), 1, pROM);
    fwrite(&packet_length, sizeof(uint8_t), 1, pROM);
    fwrite(&tileID, sizeof(uint8_t), 1, pROM);

    rom_bank_size += METADATA + 1;
}


void method1(int row, int subrow){
    uint16_t packet_adress = TILEMAP_START + subrow_start_collumn[subrow] + row * FULLTILEMAPLENGTH;
    uint8_t packet_length = subrow_lens[subrow];
    write_mode1(packet_adress, packet_length, row);
}


void method2(int row, int subrow){
    // since for 1 tile, it doesn't matter if you choose mode 1 or 2, we'll only use mode 2
    uint16_t start_adress = TILEMAP_START + subrow_start_collumn[subrow] + row * FULLTILEMAPLENGTH;
    uint16_t packet_adress;
    uint8_t packet_length;
    int counter = 0;

    while(counter<subrow_lens[subrow]-1){
        packet_adress = start_adress + counter;
        packet_length = 1;
        while(subrows[subrow][counter] == subrows[subrow][counter+1]){
            packet_length++;
            counter++;
            if(counter>=subrow_lens[subrow]-1) break;
        }
        write_mode2(packet_adress, packet_length, row);
        counter++;
    }
}


void write_repetition_method(int row, int subrow, int max_rep_len){
    uint16_t start_adress = TILEMAP_START + subrow_start_collumn[subrow] + row * FULLTILEMAPLENGTH;
    uint16_t packet_adress;
    uint16_t packet_length;

    int p1 = 0;
    int p2 = 0;
    int counter = 0;
    int rep_len = 0;

    while(counter<subrow_lens[subrow]-1){
        if(subrows[subrow][counter] == subrows[subrow][counter+1]){
            p2 = counter;
            while((counter<subrow_lens[subrow]-1) && (subrows[subrow][counter] == subrows[subrow][counter+1])){
                counter++;
            }
            rep_len = counter - p2 + 1;
            if(rep_len>max_rep_len){
                if(p2-p1>0){
                    packet_adress = start_adress + p1;
                    packet_length = p2 - p1;
                    write_mode1(packet_adress, packet_length, row);
                }
                packet_adress = start_adress + p2;
                packet_length = rep_len;
                write_mode2(packet_adress, packet_length, row);

                p1 = counter + 1;
            }
        }
        else counter++;
    }
    counter = subrow_lens[subrow];
    if(p1 != counter){
        packet_adress = start_adress + p1;
        packet_length = counter - p1;
        write_mode1(packet_adress, packet_length, row);
    }
}


void write_big_packet(int row){ 
    uint16_t packet_adress = TILEMAP_START + row*FULLTILEMAPLENGTH + subrow_start_collumn[0];
    uint8_t packet_length = TILEWIDTH-subrow_start_collumn[0];
    write_mode1(packet_adress, packet_length, row);
}

void write_small_packets(int row, int subrow){
    switch(best_methods[subrow]){
        case 1:
            method1(row, subrow);
            break;
        case 2:
            method2(row, subrow);
            break;
        case 3:
            write_repetition_method(row, subrow, 4);
            break;
        case 4:
            write_repetition_method(row, subrow, 7);
            break;
        default:
            printf("Error choosing method. Aborting...\n");
            fclose(pROM);
            exit(1);
    }
}


void process_frame(){
    for(int row=0; row<TILEHEIGHT; row++){
        for(int i=0; i<TILEWIDTH; i++) {
            if(current_frame[row*TILEWIDTH+i] != previous_frame[row*TILEWIDTH+i]) row_buffer[i] = current_frame[row*TILEWIDTH+i];
            else row_buffer[i] = NO_TILE;
        }
        load_subrows();
        if(num_subrows == 0) continue;

        // check if one big packet is more efficient
        int scores_sum = 0;
        for(int subrow=0; subrow<num_subrows; subrow++) {
            test_subrow(subrow);
            scores_sum += best_scores[subrow];
        }
        if(scores_sum >= METADATA+TILEWIDTH-subrow_start_collumn[0]) {
            write_big_packet(row);
        }
        else for(int subrow=0; subrow<num_subrows; subrow++){
            write_small_packets(row, subrow);
        }
    }
    fwrite(&FRAME_END, sizeof(uint8_t), 1, pROM);
    
    rom_bank_size += 1;
}


int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Wrong argument count, pass arguments like so: \n./video-encoder.sh [input frame folder (.bin)] [output rom-bank folder (.bin)]\n");
        return 1;
    }

    change_rom_bank(argv);
    load_frame(argv);
    set_previous_frame();

    while(next_frame_null(argv) == false){
        while((rom_bank_full() == false) && (next_frame_null(argv) == false)){
            load_frame(argv);
            process_frame();
            set_previous_frame();
            printf("frame%d done\n", frame_counter);
        }
        fwrite(&ROM_END, sizeof(uint8_t), 1, pROM);
        change_rom_bank(argv);
    }

    printf("Successfully converted!\n");
    fclose(pROM);
    return 0;
}
