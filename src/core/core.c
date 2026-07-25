#include <stddef.h>
#include "../include/my_types.h"

// max rom size = 0xFFF - 0x200
#define MAX_ROM_SIZE 3584

#if defined(SMALL)
# define VRAM_SIZE 256
# define LUT
#elif defined(FAST)
# define VRAM_SIZE 8192
# define LUT u32 lut[256][8] __attribute((aligned(8)));
#else
# define VRAM_SIZE 2048
#define LUT
#endif

#define RAM_SIZE 4096

typedef struct{
    u8 ram[RAM_SIZE] __attribute__((aligned(8)));  // RAM slot
    u8 vram[VRAM_SIZE] __attribute__((aligned(8))); // 1 bit per pixel for SMALL mode and u32 per pixel for FAST mode and u8 per pixel for normal mode
    LUT
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

int chip_init(chip8 *chip,u8 rand){
  if (chip == NULL || rand == 0) return -1;
  i16 i;
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

  #if defined (FAST)
  int j;
  for (i=0;i<256;i++){
    for (j=7;j>=0;j--){
      chip->lut[i][7-j] = ((i&(1<<j))>>j)*(-1);
    }
  }
  #endif
  
  return 0;
}

int load_rom(chip8 *chip,string8 rom){
  // the ROM buffer read by the platform must end by an EOF
  // only classic chip 8 mode will be eligible for now !!!
  if (chip == NULL || rom.size > MAX_ROM_SIZE) return -1;
  size_t i;
  for (i=0;i<rom.size;i++){
    chip->ram[i+0x200] = rom.data[i];
  }
  return 0;
}
 
// a pseudo u8 number generator from the chip random variable
inline __attribute((always_inline)) u8 random_byte(u8 *seed){
  if (*seed == 0) *seed = (u8)0b0110101101;
  int x = *seed;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  *seed = x;
  return (u8)(x & 0xFF);
}



int run(chip8 *chip){
  if (chip == NULL) return -1;
  u16 inst = chip->ram[chip->pc] << 8 | chip->ram[chip->pc+1];
  // assuming the 16bit instruction is in this format "abcd" where a containing MS4bs
  u8 a = inst >> 12;
  u8 b = (inst & 0x0F00) >> 8;
  u8 c = (inst & 0x00F0) >> 4;
  u8 d = inst & 0x000F;
  
  chip->pc += 2;
  
  #if defined(SMALL)
  switch (a){
    case 0: goto label_0;
    case 1: goto label_1;
    case 2: goto label_2;
    case 3: goto label_3;
    case 4: goto label_4;
    case 5: goto label_5;
    case 6: goto label_6;
    case 7: goto label_7;
    case 8: goto label_8;
    case 9: goto label_9;
    case 0xA: goto label_A;
    case 0xB: goto label_B;
    case 0xC: goto label_C;
    case 0xD: goto label_D;
    case 0xE: goto label_E;
    case 0xF: goto label_F;
    default: return -1;
  }

  #else
  // for FAST and DEFAULT modes 
  const void* array[16] = {
    &&label_0,&&label_1,&&label_2,&&label_3,
    &&label_4,&&label_5,&&label_6,&&label_7,
    &&label_8,&&label_9,&&label_A,&&label_B,
    &&label_C,&&label_D,&&label_E,&&label_F
  };
  goto *array[a];
  #endif
  
  label_0:{
    // since the 0NNN sys jump was a thing for chip 8, i won't return -1 in that case
    // CLS: clear the display
    if (b == 0 && c == 0xE && d == 0){
      int i;
      int chunck_size = VRAM_SIZE/sizeof(size_t);
      size_t *chunck = (size_t *)(chip->vram);
      for (i=0;i<chunck_size;i++){
        chunck[i] = 0;
      }
    }
    // RET: return from a subroutine
    if (b == 0 && c == 0xE && d == 0xE){
      chip->pc = chip->stack[(chip->sp)--];
    }
    return 0;
  }

  label_1:{
    chip->pc = (b << 8) | (c << 4) | (d);
    return 0;
  }

  label_2:{
    chip->stack[++(chip->sp)] = chip->pc;
    goto label_1;
    return 0;
  }

  label_3:{
    u8 value = (c << 4) | d;
    chip->pc += 2*(chip->v[b] == value);
    return 0;
  }

  label_4:{
    u8 value = (c << 4) | d;
    chip->pc += 2*(chip->v[b] != value);
    return 0;
  }

  label_5:{
    if (d != 0) return -1;
    chip->pc += 2*(chip->v[b] == chip->v[c]);
    return 0;
  }

  label_6:{
    chip->v[b] = (c << 4) | d;
    return 0;
  }

  label_7:{
    chip->v[b] += (c << 4) | d;
    return 0;
  }

  label_8:{
    u16 vx = chip->v[b];
    u16 vy = chip->v[c];
    switch (d){
      case 0:
        vx = vy;
        chip->v[0xF] = 0;
        break;
      case 1:
        vx |= vy;
        chip->v[0xF] = 0;
        break;
      case 2:
        vx &= vy;
        chip->v[0xF] = 0;
        break;
      case 3:
        vx ^= vy;
        chip->v[0xF] = 0;
        break;
      case 4:
        vx += vy;
        chip->v[0xF] = vx >> 8;
        vx &= 0xFF;
        break;
      case 5:
        chip->v[0xF] = (vx >= vy);
        vx = (vx - vy) & 0xFF;
        break; 
      case 6:
        vx=vy;
        chip->v[0xF] = vx & 0x01;
        vx = vx >> 1;
        break;
      case 7:
        chip->v[0xF] = (vy >= vx);
        vx = (vy - vx) & 0xFF;
        break;
      case 0xE:
        vx = vy;
        chip->v[0xF] = vx >> 7;
        vx = (vx << 1) & 0xFF;
        break;
      default: return -1;
    }
    chip->v[b] = (u8)vx;
    return 0;
  }

  label_9:{
    if (d != 0) return -1;
    chip->pc += 2*(chip->v[b] != chip->v[c]);
    return 0;
  }

  label_A:{
    chip->i = (b << 8) | (c << 4) | d;
    return 0;
  }

  label_B:{
    chip->pc = ((b << 8) | (c << 4) | d) + chip->v[0];
    return 0;
  }

  label_C:{
    u8 random = random_byte(&(chip->rand));
    chip->v[b] = random & ((c << 4) | d);
    return 0;
  }

  #if defined(SMALL)
  label_D:{
    u16 a = chip->i;
    u8 x = chip->v[b];
    u8 y = chip->v[c];
    u8 n = d;
    u8 i,row,byte,right,left,index1,index2,vram_slot1,vram_slot2,b1,b2,z1,z2;
    byte = x >> 3;  // /8
    right = x - (byte << 3); // *8
    left = 8 - right;
    u8 collision = 0;
    b1 = byte & 0b0111;  // %8
    b2 = (byte+1) & 0b0111;  // %8
    u8 y1 = (y & 0b00011111) << 3; // (y%32)*8
    for (i=0;i<n;i++){
      row = chip->ram[a++];
      z1 = ((row >> right) & 0xFF);
      z2 = ((row << left) & 0xFF);
      index1 = b1 + y1;
      index2 = b2 + y1;
      vram_slot1 = chip->vram[index1];
      vram_slot2 = chip->vram[index2];
      collision |= vram_slot1 & z1;
      chip->vram[index1] = vram_slot1 ^ z1;
      if (right != 0){
        collision |= vram_slot2 & z2;
        chip->vram[index2] = vram_slot2 ^ z2;
      }
      y1 = (y1 + 8) & 0xFF;  // %256
    }
    chip->v[0xF] = (collision != 0);
    return 0;
  }

  #elif defined(FAST)
  label_D:{
    u16 i = chip->i;
    u8 x = chip->v[b];
    u8 y = chip->v[c];
    u8 n = d;
    int j,k;
    u8 byte;
    u32 data;
    int index_y = (y&31) << 6; // *64
    u32 *array = (u32*)chip->vram;
    int collision = 0;
    for (j=0;j<n;j++){
      byte = chip->ram[i];
      for (k=0;k<8;k++){
        data = chip->lut[byte][k];
        collision |= array[((x+k)&63)+index_y] & data;
        array[((x+k)&63)+index_y] ^= data;
      }
      i++;
      index_y = (index_y + 64) & 2047;
    }
    chip->v[0xF] = (collision != 0);
    return 0;
  }

  #else
  label_D:{
    u16 i = chip->i;
    u8 x = chip->v[b];
    u8 y = chip->v[c];
    u8 n = d;
    int j;
    i8 k;
    u8 byte;
    int collision = 0;
    u16 index_y = (y&31) << 6;
    int index_x;
    u32 data;
    u8 *array = chip->vram;
    for (j=0;j<n;j++){
      byte = chip->ram[i];
      for (k=7;k>=0;k--){
        index_x = index_y + ((x+7-k)&63);
        data = ((-1)*((byte & (1 << k)) >> k));
        collision |= array[index_x] & data;
        array[index_x] ^= data;
      }
      index_y = (index_y+64)&2047;
      i++;
    }
    chip->v[0xF] = (collision != 0);
    return 0;
    }
  #endif

// NOTE: this may be optimized just check Gemini to check
  label_E:{
    if (c == 9 && d == 0xE){
      if (((chip->key) & (1 << chip->v[b])) > 0){
        chip->pc += 2;
      }
      return 0;
    }else if (c == 0xA && d == 1){
      if (((chip->key) & (1 << chip->v[b])) == 0){
        chip->pc += 2;
      }
      return 0;
    }else{
      return -1;
    }
    }
    

  label_F:{
    int a;  
    u8 x = (c << 4) | d;
    switch (x){
      case 0x07:
        chip->v[b]=chip->dt;
        break;
      case 0x0A:
        if (chip->key == 0){
          chip->pc -= 2;
        }else{
          chip->v[b] = __builtin_ctz(chip->key);
        }
        break;  // the algorithme of reading the keyboard input must be slower than the execution loop to avoid missing key input
      case 0x15:
        chip->dt = chip->v[b];
        break;
      case 0x18:
        chip->st = chip->v[b];
        break;
      case 0x1E:
        chip->i += chip->v[b];
        break;
      case 0x29:
        chip->i = chip->v[b]*5;
        break;
      case 0x33:
        a = chip->i;
        int v = chip->v[b];
        #if defined(SMALL)
        // for processor that don't support division
        u8 hundreds = 0, tens = 0;
        while (v >= 100) { v -= 100; hundreds++; }
        while (v >= 10)  { v -= 10; tens++; }
        chip->ram[a++] = hundreds;
        chip->ram[a++] = tens;
        chip->ram[a] = v;
        #else
        // for processors that support division
        chip->ram[a++] = v/100;
        v %= 100;
        chip->ram[a++] = v/10;
        v %= 10;
        chip->ram[a] = v;
        #endif
        break;
      case 0x55:{
        a = chip->i;
        u8 *ram = chip->ram+a;
        u8 *reg = chip->v;
        for (int i=0;i<=b;i++){
          *(ram + i) = *(reg + i);
        }
        chip->i = a+b+1;
        break;
      }
      case 0x65:{
        a = chip->i;
        u8 *ram = chip->ram+a;
        u8 *reg = chip->v;
        for (int i=0;i<=b;i++){
          *(reg + i) = *(ram + i);
        }
        chip->i = a+b+1;
        break;
      }
      default: return -1;
    }
    return 0;
    }
}

#if defined(SMALL)
int read_vram(chip8 *chip,void *pixel_buffer){
  if (chip == NULL || pixel_buffer == NULL) return -1;
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

#elif defined(FAST)
int read_vram(chip8 *chip,void *pixel_buffer){
  if (chip == NULL || pixel_buffer == NULL) return -1;
  u32* buffer = (u32*)(pixel_buffer);
  u32 *array = (u32*)(chip->vram);
  int i;
  for (i=0;i<(VRAM_SIZE/4);i++){
    buffer[i] = array[i];
  }
  return 0;
}
#else
int read_vram(chip8 *chip,void *pixel_buffer){
  if (chip == NULL || pixel_buffer == NULL) return -1;
  u32* buffer = (u32*)(pixel_buffer);
  u8 *array = chip->vram;
  int i;
  for (i=0;i<VRAM_SIZE;i++){
    buffer[i] = (array[i]&1)*(-1);
  }
  return 0;
}
#endif
