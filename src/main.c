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

// hack needed by SDL
void sysconf(){
  /* empty */
}

int main(void) {

  //  SDL_Surface *screen;

  /*  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return -1;
    }*/

  /*  if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_FULLSCREEN|SDL_HWSURFACE))) {
    SDL_Quit();
    return -1;
    }
    SDL_Flip(screen);*/
  xenos_init(VIDEO_MODE_AUTO);
  console_init();
  

  chip8_init();
  load_rom("sda:/data/");
  printf("Hello, world!\n");

  //  SDL_Quit();
  return 0;
}
