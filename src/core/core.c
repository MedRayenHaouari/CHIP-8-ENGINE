#include <stddef.h>
#include "../include/my_types.h"

#define VRAM_SIZE 256
#define RAM_SIZE 4096

typedef struct{
    u8 ram[RAM_SIZE] __attribute__((aligned(8)));  // RAM slot
    u8 vram[VRAM_SIZE] __attribute__((aligned(8))); // Video RAM
    u16 key;  // keyboard keys with LSB at index 0
    u16 i;  // the 16bit I register
    u16 pc;  // program counter
    u8 v[16];  // general pupose Vx registers
    u16 stack[16];  // stack
    u8 rand;  // random number
    u8 st;  // sound timer
    u8 dt;  // delay timer
    i8 sp;  // stack pointer
} chip8;

typedef struct{
  char *data;
  size_t size;
} string8;

#include "instruction_decoder.c"

int chip_init(chip8 *chip,u8 rand){
  if (chip == NULL || rand == 0) return -1;
  u16 i;
  u16 chunck_size = RAM_SIZE/sizeof(size_t);
  size_t *chunck = (size_t*)(chip->ram);
  for (i=0;i<chunck_size;i++){
    chunck[i] = 0;
  }
  chunck_size = VRAM_SIZE/sizeof(size_t);
  chunck = (size_t*)(chip->vram);
  for (i=0;i<chunck_size;i++){
    chunck[i] = 0;
  }
  u8 sprites[80] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
  for (i=0;i<80;i++){
    chip->ram[i] = sprites[i];
  }
  for (i=0;i<16;i++){
    chip->v[i] = 0;
  }
  // for the classic chip 8
  chip->pc = 0x200;
  chip->sp = -1;
  chip->rand = rand;
  chip->dt = 0;
  chip->st = 0;
  return 0;
}

int load_rom(chip8 *chip,string8 rom){
  // the ROM buffer read by the platform must end by an EOF
  // only classic chip 8 mode will be eligible for now !!!
  if (chip == NULL || rom.size > 3583) return -1;
  size_t i;
  for (i=0;i<rom.size;i++){
    chip->ram[i+0x200] = rom.data[i];
  }
  return 0;
}

int run(chip8 *chip){
  if (chip == NULL) return -1;
  u16 inst = chip->ram[chip->pc] << 8 | chip->ram[chip->pc+1];
  // assuming the 16bit instruction is in this format "abcd" where a containing MS4bs
  u8 a = inst >> 12;
  u8 b = (inst & 0x0F00) >> 8;
  u8 c = (inst & 0x00F0) >> 4;
  u8 d = inst & 0x000F;
  if (instruction_fetcher(chip,a,b,c,d) < 0){
    return -1;
  }
  return 0;
}

int read_vram(chip8 *chip,void *pixel_buffer){
  if (chip == NULL) return -1;
  u8 *array = (u8*)pixel_buffer;
  int i,index_i,index_j,index_k;
  u8 byte,j,k,res;
  for (i=0;i<=255;i++){
    index_i = i*32;
    byte = chip->vram[i];
    for (j=0;j<8;j++){
      index_j = j*4;
      res = (byte >> (7-j)) & 1 ? 0xFF : 0;
      index_k = index_i + index_j;
      for (k=0;k<3;k++){
        array[index_k + k] = res;
      }
      array[index_k + 3] = 0xFF;
    }
  }
  return 0;
}
