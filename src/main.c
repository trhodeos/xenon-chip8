#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <xenos/xenos.h>
#include <console/console.h>
#include <usb/usbmain.h>
#include <SDL/SDL.h>
#include <time/time.h>

#define WIDTH  640
#define HEIGHT 480
#define DEPTH  32

// 60hz
#define UPDATE_STEP (1.0/60)

int is_running = 0;
SDL_Surface *screen;

// hack needed by SDL
void sysconf(){
  /* empty */
}
void set_pixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
  Uint32 *pixmem32;
  Uint32 colour;  
 
  colour = SDL_MapRGB( screen->format, r, g, b );
  
  pixmem32 = (Uint32*) screen->pixels  + y + x;
  *pixmem32 = colour;
}

void render(SDL_Surface *screen) {
  int i, j, k;
  byte temp;
  if (SDL_MUSTLOCK(screen)) {
    if (SDL_LockSurface(screen) < 0) {
      return;
    }
  }

  for (i = 0; i < GFX_HEIGHT; i++) {
    for (j = 0; j < GFX_WIDTH; j++) {
      temp = gfx[i*GFX_WIDTH + j];
      if (temp) {
        set_pixel(screen, j*8 + k, i, 0xFF, 0xFF, 0xFF);
      } else {
        set_pixel(screen, j*8 + k, i, 0x00, 0x00, 0x00);
      }
    }
  }

  if ( SDL_MUSTLOCK(screen) ) {
    SDL_UnlockSurface(screen);
  }
  SDL_UpdateRect(screen, 0, 0, GFX_WIDTH, GFX_HEIGHT);
  SDL_Flip(screen);
}

void main_loop() {
  double accumulator;
  double delta;
  time_t current_time, last_time;
  SDL_Event event;

  printf("starting main loop\n");

  accumulator = 0;
  is_running = 1;
  
  last_time = time();//SDL_GetTicks();
  while (is_running) {
    current_time = time();
    // delta in seconds, not ms
    delta = (current_time - last_time) / 1000.0;
    last_time = current_time;

    // hack to allow response to breakpoints/halted execution
    if (delta > 1.0f)
      delta = 0.0016f;

    //    while (SDL_PollEvent(&event)) {
      //      HandleEvent(event, delta);
    //    }
    
    accumulator += delta;
    if (accumulator > UPDATE_STEP)
    {
      emulate_cycle();
      render(screen);

      accumulator -= UPDATE_STEP;
    }
  }
}


int main() {


  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return -1;
  }

  if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_FULLSCREEN|SDL_HWSURFACE))) {
    SDL_Quit();
    return -1;
  }
  
  // clear out garbage
  SDL_Flip(screen);

  xenos_init(VIDEO_MODE_AUTO);
  
  usb_init();
  usb_do_poll();

  console_init();  

  chip8_init();
  printf("done initializing\n");
  load_rom("uda:/data/pong.ch8");
  printf("done loading rom\n");

  main_loop();

  SDL_Quit();
  return 0;
}
