#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <xenos/xenos.h>
#include <console/console.h>

#include <SDL/SDL.h>

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
  // TODO
}

void main_loop() {
  double accumulator;
  float delta, current_time, last_time;
  SDL_Event event;

  accumulator = 0;
  is_running = 1;
  
  last_time = SDL_GetTicks();
  while (is_running) {
    current_time = SDL_GetTicks();
    // delta in seconds, not ms
    delta = (current_time - last_time) / 1000;
    last_time = current_time;

    // hack to allow response to breakpoints/halted execution
    if (delta > 1.0f)
      delta = 0.0016f;

    while (SDL_PollEvent(&event)) {
      //      HandleEvent(event, delta);
    }

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

  SDL_Surface *screen;

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
  console_init();  

  chip8_init();
  //  load_rom("sda:/data/");

  main_loop();

  SDL_Quit();
  return 0;
}
