#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../core/core.c"
#include "simulate.c"
#include "../include/my_colors.h"
#include "../include/my_symbols.h"
#include "../include/my_types.h"

#define SCALE 15

int main(){
  printf("Starting the Emulator...\n");

  // initialization phase
  chip8 chip;
  u8 random = (rand() & 0xFF);
  if (chip_init(&chip,random) < 0){
    perror("[" COLOR_RED SYM_CROSS COLOR_RESET "] Error While Initializing the Chip\n");
    return -1;
  }
  printf("[" COLOR_GREEN SYM_CHECK COLOR_RESET "] Chip Initialized\n");

  // loading the rom
  char *buffer = (char*)calloc(3584,sizeof(char));
  if (!buffer){
    perror("[" COLOR_RED SYM_CROSS COLOR_RESET "] Error While Allocation Buffer\n");
    return -1;
  }
  string8 rom;
  FILE *f = fopen("../../roms/Lunar Lander (Udo Pernisz, 1979).ch8","rb");
  if (!f){    
    perror("[" COLOR_RED SYM_CROSS COLOR_RESET "] Error While Opening File\n");
    free(buffer);
    return -1;
  }
  size_t rom_size = fread(buffer,1,3584,f);
  if (fread(buffer,1,3584,f) > 0){
    perror("[" COLOR_RED SYM_CROSS COLOR_RESET "] Error While Reading File\n");
    fclose(f);
    free(buffer);
    return -1;
  }
  fclose(f);
  rom.size = rom_size;
  rom.data = buffer;
  printf("[" COLOR_GREEN SYM_CHECK COLOR_RESET "] ROM Read Successfully\n");
  if (load_rom(&chip,rom) < 0){
    perror("[" COLOR_RED SYM_CROSS COLOR_RESET "] Error While Loading ROM to the Chip\n");
    free(buffer);
    return -1;
  }
  printf("[" COLOR_GREEN SYM_CHECK COLOR_RESET "] ROM Loaded to Chip Successfully\n");

  // running the chip
  if (simulate(&chip,SCALE) < 0){
    perror("[" COLOR_RED SYM_CROSS COLOR_RESET "] Error During Simulation\n");
    free(buffer);
    return -1;
  }

  free(buffer);
  return 0;
}
