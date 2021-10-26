#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void sdl(int code) {
  if (code < 0) {
    printf("%s\n", SDL_GetError());
  }
}

void sdl_created(void* ptr) {
  if (ptr == NULL) {
    printf("%s\n", SDL_GetError());
  }
}

int main() {

  SDL_Window* win;
  SDL_Renderer* rend;

  sdl(SDL_Init(SDL_INIT_VIDEO != 0));
  win = SDL_CreateWindow("Tic Tac Toe", 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
    WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  sdl_created(&win);
  rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  sdl_created(&rend);

  int quit = 0;
  while (quit == 0) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT:
          quit = 1;
          break;
        default:
          break;
      }
    }
    SDL_RenderClear(rend);
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    SDL_RenderPresent(rend);
  }
}
