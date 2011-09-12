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

  SDL_Surface *screen;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return -1;
  }

  if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_FULLSCREEN|SDL_HWSURFACE))) {
    SDL_Quit();
    return -1;
  }

  console_init();
  printf("Hello, world!\n");

  SDL_Quit();
  return 0;
}
