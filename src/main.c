#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <xenos/xenos.h>
#include <console/console.h>
#include <usb/usbmain.h>
#include <ppc/timebase.h>
//#include <SDL/SDL.h>

#define WIDTH  640
#define HEIGHT 480
#define DEPTH  32

// 60hz
#define UPDATE_STEP (1.0/60)

int is_running = 0;

// hack needed by SDL
void sysconf(){
  /* empty */
}

void render() {
  int i, j, k;
  byte temp;
  if (draw_flag) {
    console_clrscr();
    for (i = 0; i < GFX_HEIGHT; i++) {
      for (j = 0; j < GFX_WIDTH; j++) {
        temp = gfx[i*GFX_WIDTH + j];
        for (k = 0; k < 8; k++) {
          if (temp & 0x80) 
            printf("*");
          else
            printf(" ");
          temp = temp << 1;
        }
      }
      printf("\n");
    }
    draw_flag = 0;
  }
}

void main_loop() {
  double accumulator;
  double delta;
  uint64_t current_time, last_time;
  //  SDL_Event event;

  printf("starting main loop\n");

  accumulator = 0;
  is_running = 1;
  
  last_time = mftb();
  while (is_running) {
    current_time = mftb();
    // delta in seconds, not ms
    delta = tb_diff_msec(current_time, last_time) / 1000.0;
    last_time = current_time;

    // hack to allow response to breakpoints/halted execution
    //    if (delta > 1.0f)
    //      delta = 0.0016f;
    //    printf("%f\n", delta);
    //    while (SDL_PollEvent(&event)) {
      //      HandleEvent(event, delta);
    //    }

    accumulator += delta;
    if (accumulator > UPDATE_STEP)
    {
      emulate_cycle();
      render();

      accumulator -= UPDATE_STEP;
    }
  }
}


int main() {

  //SDL_Surface *screen;

  //  if (SDL_Init(SDL_INIT_TIMER) < 0) {
  //    return -1;
  //  }

  /*  if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_FULLSCREEN|SDL_HWSURFACE))) {
    SDL_Quit();
    return -1;
    }*/
  
  // clear out garbage
  //  SDL_Flip(screen);

  xenos_init(VIDEO_MODE_AUTO);
  
  usb_init();
  usb_do_poll();

  console_init();  

  chip8_init();
  printf("done initializing\n");
  load_rom("uda:/data/pong.ch8");
  printf("done loading rom\n");
  main_loop();

  //  SDL_Quit();
  return 0;
}
