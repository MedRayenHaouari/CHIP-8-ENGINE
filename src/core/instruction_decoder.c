// NOTE: Original COSMAC VIP specification are assumed here

#include "../include/my_types.h"

static u8 random_byte(u8 *seed) {
  if (*seed == 0) return -1;
  int x = *seed;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  *seed = x;
  return (u8)(x & 0xFF);
}

typedef int (*layer1_t)(chip8 *chip,u8 b,u8 c,u8 d);

static int func0(chip8 *chip,u8 b,u8 c,u8 d){
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

static int func1(chip8 *chip,u8 b,u8 c,u8 d){
  chip->pc = (b << 8) | (c << 4) | (d);
  return 0;
}

static int func2(chip8 *chip,u8 b,u8 c,u8 d){
  chip->stack[++(chip->sp)] = chip->pc;
  chip->pc = (b << 8) | (c << 4) | (d);
  return 0;
}

static int func3(chip8 *chip,u8 b,u8 c,u8 d){
  u8 value = (c << 4) | d;
  if (chip->v[b] == value){
    chip->pc += 2;
  }
  return 0;
}

static int func4(chip8 *chip,u8 b,u8 c,u8 d){
  u8 value = (c << 4) | d;
  if (chip->v[b] != value){
    chip->pc += 2;
  }
  return 0;
}

static int func5(chip8 *chip,u8 b,u8 c,u8 d){
  if (d != 0) return -1;
  if (chip->v[b] == chip->v[c]){
    chip->pc += 2;
  }
  return 0;
}

static int func6(chip8 *chip,u8 b,u8 c,u8 d){
  chip->v[b] = (c << 4) | d;
  return 0;
}

static int func7(chip8 *chip,u8 b,u8 c,u8 d){
  chip->v[b] += (c << 4) | d;
  return 0;
}

static int func8(chip8 *chip,u8 b,u8 c,u8 d){
  u16 vx = chip->v[b];
  u16 vy = chip->v[c];
  switch (d){
    case 0: vx = vy; chip->v[0xF] = 0;  break;
    case 1: vx |= vy; chip->v[0xF] = 0; break;
    case 2: vx &= vy; chip->v[0xF] = 0; break;
    case 3: vx ^= vy; chip->v[0xF] = 0; break;
    case 4: vx += vy; chip->v[0xF] = vx >> 8 /* set carry */; vx &= 0xFF; break;
    case 5: chip->v[0xF] = (vx >= vy) ? 1 : 0; vx = (vx - vy) & 0xFF; break; 
    case 6: vx=vy; chip->v[0xF] = vx & 0x01; vx = vx >> 1; break;
    case 7: chip->v[0xF] = (vy >= vx) ? 1 : 0; vx = (vy - vx) & 0xFF; break;
    case 0xE: vx = vy; chip->v[0xF] = vx >> 7; vx = (vx << 1) & 0xFF; break;
    default: return -1;
  }
  chip->v[b] = (u8)vx;
  return 0;
}

static int func9(chip8 *chip,u8 b,u8 c,u8 d){
  if (d != 0) return -1;
  if (chip->v[b] != chip->v[c]){
    chip->pc += 2;
  }
  return 0;
}

static int funcA(chip8 *chip,u8 b,u8 c,u8 d){
  chip->i = (b << 8) | (c << 4) | d;
  return 0;
}

static int funcB(chip8 *chip,u8 b,u8 c,u8 d){
  chip->pc = ((b << 8) | (c << 4) | d) + chip->v[0];
  return 0;
}

static int funcC(chip8 *chip,u8 b,u8 c,u8 d){
  u8 random = random_byte(&(chip->rand));
  chip->v[b] = random & ((c << 4) | d);
  return 0;
}

static int funcD(chip8 *chip,u8 b,u8 c,u8 d){
  // do not touch! peace of art
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
    // the %8 cuz the total number of columns in vram = 8 so that if we have to write off the boundries we get get back to the beginning
    // same for %32 so that if y > 31 we return to the top to write
    // when the vram byte and the row byte to xor if we AND them together and get a non 0 value then there must be two 1s overlapping then we have collision
    // b1 b2 and z are simply abreviations for optimization purposes if you wanna understand the code replace them with their definitions
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
  chip->v[0xF] = (collision > 0 ? 1 : 0);
  return 0;
}

static int funcE(chip8 *chip,u8 b,u8 c,u8 d){
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

static int funcF(chip8 *chip,u8 b,u8 c,u8 d){
  u8 x = (c << 4) | d;
  switch (x){
    case 0x07: chip->v[b]=chip->dt; break;
    case 0x0A: if (chip->key == 0){
        chip->pc -= 2;
      }else{
        chip->v[b] = __builtin_ctz(chip->key);
      } break; // updating the key map integer must be a bit slow
    case 0x15: chip->dt = chip->v[b]; break;
    case 0x18: chip->st = chip->v[b]; break;
    case 0x1E: chip->i += chip->v[b]; break;
    case 0x29: chip->i = chip->v[b]*5; break;
    case 0x33: {
        int a = chip->i;
        int v = chip->v[b];
        chip->ram[a++] = v/100;
        v %= 100;
        chip->ram[a++] = v/10;
        v %= 10;
        chip->ram[a] = v;
        break;
      }
    case 0x55: {
        int a = chip->i;
        for (int i=0;i<=b;i++){
          chip->ram[a++] = chip->v[i];
        }
        chip->i = a;
        break;
      }
    case 0x65: {
        int a = chip->i;
        for (int i=0;i<=b;i++){
          chip->v[i] = chip->ram[a++];
        }
        chip->i = a;
        break;
      }
    default: return -1;
  }
  return 0;
}

int instruction_fetcher(chip8 *chip,u8 a,u8 b,u8 c,u8 d){
  static layer1_t layer1[16] = {func0,func1,func2,func3,func4,func5,func6,func7,func8,func9,funcA,funcB,funcC,funcD,funcE,funcF};
  chip->pc += 2;
  if (layer1[a](chip,b,c,d) < 0){
    return -1;
  }
  return 0;
}
