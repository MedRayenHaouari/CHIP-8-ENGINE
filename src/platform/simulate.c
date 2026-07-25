#include <stdio.h>
#include <raylib.h>
#include "../include/my_types.h"
#include "../include/my_colors.h"
#include "../include/my_symbols.h"

#define TARGET_FPS 60
#define CPU_FREQ 700
#define INST_PER_FRAME (CPU_FREQ/TARGET_FPS)

void update_key_inputs(chip8 *chip) {
    chip->key = 0;
    if (IsKeyDown(KEY_ONE))   chip->key |= (1 << 0x1);
    if (IsKeyDown(KEY_TWO))   chip->key |= (1 << 0x2);
    if (IsKeyDown(KEY_THREE)) chip->key |= (1 << 0x3);
    if (IsKeyDown(KEY_FOUR))  chip->key |= (1 << 0x4);
    if (IsKeyDown(KEY_FIVE))     chip->key |= (1 << 0x5);
    if (IsKeyDown(KEY_SIX))     chip->key |= (1 << 0x6);
    if (IsKeyDown(KEY_SEVEN))     chip->key |= (1 << 0x7);
    if (IsKeyDown(KEY_EIGHT))     chip->key |= (1 << 0x8);
    if (IsKeyDown(KEY_NINE))     chip->key |= (1 << 0x9);
    if (IsKeyDown(KEY_A))     chip->key |= (1 << 0xA);
    if (IsKeyDown(KEY_B))     chip->key |= (1 << 0xB);
    if (IsKeyDown(KEY_C))     chip->key |= (1 << 0xC);
    if (IsKeyDown(KEY_D))     chip->key |= (1 << 0xD);
    if (IsKeyDown(KEY_E))     chip->key |= (1 << 0xE);
    if (IsKeyDown(KEY_F))     chip->key |= (1 << 0xF);
    if (IsKeyDown(KEY_ZERO))     chip->key |= (1 << 0x0);
}

int simulate(chip8 *chip,int scale){
  InitWindow(64*scale,32*scale,"CHIP-8 EMULATOR");
  SetTargetFPS(TARGET_FPS);
  Color screen_pixels[64*32]={0};
  Image img={
    .data = screen_pixels,
    .width = 64,
    .height = 32,
    .mipmaps = 1,
    .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
  };
  Texture2D screen_texture = LoadTextureFromImage(img);
  SetTextureFilter(screen_texture,TEXTURE_FILTER_POINT);
  int i;
  
  while (!WindowShouldClose()){
    int debug_key = GetKeyPressed();
    if (debug_key > 0) {
        printf("Detected Key Code: %d\n", debug_key);
    }
    update_key_inputs(chip);
    for (i=0;i<INST_PER_FRAME;i++){
      if (run(chip) < 0){
        perror("[" COLOR_RED SYM_CROSS COLOR_RESET "] Error While Running the Chip\n");
        return -1;
      }
    }
    read_vram(chip,screen_pixels);
    UpdateTexture(screen_texture,screen_pixels);
    
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextureEx(screen_texture,(Vector2){0, 0},0.0f,scale, WHITE);
    EndDrawing();
    if (chip->dt > 0) chip->dt--;
    if (chip->st > 0) chip->st--;
  }
  UnloadTexture(screen_texture);
  CloseWindow();
  return 0;
}
