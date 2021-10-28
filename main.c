#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define N 3
#define CELL_WIDTH (WINDOW_WIDTH / N)
#define CELL_HEIGHT (WINDOW_HEIGHT / N)

enum Player { _, X, O };

void sdl(int code) {
  if (code < 0) {
    printf("%s\n", SDL_GetError());
    abort();
  }
}

void sdl_created(void* ptr) {
  if (ptr == NULL) {
    printf("%s\n", SDL_GetError());
    abort();
  }
}

void drawGrid(SDL_Renderer* rend) {
  SDL_SetRenderDrawColor(rend, 150, 150, 150, 255);
  // draw rows
  for (int i = 1; i < N; i++) {
    SDL_RenderDrawLine(rend,
      0, i * CELL_HEIGHT,
      WINDOW_WIDTH, i * CELL_HEIGHT
    );
  }
  // draw columns
  for (int i = 1; i < N; i++) {
    SDL_RenderDrawLine(rend,
      i * CELL_WIDTH, 0,
      i * CELL_WIDTH, WINDOW_HEIGHT
    );
  }
}

void drawX(SDL_Renderer* rend, int i, int j) {
  SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
  SDL_RenderDrawLine(rend,
    j * CELL_WIDTH, i * CELL_HEIGHT,
    (j + 1) * CELL_WIDTH, (i + 1) * CELL_HEIGHT 
  );
  SDL_RenderDrawLine(rend,
    (j + 1) * CELL_WIDTH, i * CELL_HEIGHT,
    j * CELL_WIDTH, (i + 1) * CELL_HEIGHT 
  );
}

void drawO(SDL_Renderer* rend, int i, int j) {
  SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
  SDL_RenderDrawLine(rend,
    j * CELL_WIDTH, i * CELL_HEIGHT,
    (j + 1) * CELL_WIDTH, (i + 1) * CELL_HEIGHT 
  );
  SDL_RenderDrawLine(rend,
    (j + 1) * CELL_WIDTH, i * CELL_HEIGHT,
    j * CELL_WIDTH, (i + 1) * CELL_HEIGHT 
  );
} 

void drawBoard(SDL_Renderer* rend, int board[N][N]) {
  drawGrid(rend);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (board[i][j] == X) {
        drawX(rend, i, j);
      }
      if (board[i][j] == O) {
        drawO(rend, i, j);
      }
    }
  }
}

void makeMove(enum Player player, int board[N][N], int x, int y) {
  int i = y / CELL_HEIGHT;
  int j = x / CELL_WIDTH;
  board[i][j] = player;
}

void switchPlayer(enum Player* player) {
  if (*player == X) {
    *player = O;
  } else if (*player == O) {
    *player = X;
  }
}

int theGameIsOver() {
  return 0;
}

void gameOver() {
}

int main() {
  int board[N][N] = {
    {_, _, _},
    {_, _, _},
    {_, _, _}
  };

  enum Player player = X;

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
        case SDL_MOUSEBUTTONDOWN:
          makeMove(player, board, event.button.x, event.button.y);
          if (theGameIsOver()) {
            gameOver();
          } else {
            switchPlayer(&player);
          }
          break;
        default: break;
      }
    }

    // background color
    SDL_SetRenderDrawColor(rend, 70, 70, 70, 255);

    SDL_RenderClear(rend);
    drawBoard(rend, board);
    SDL_RenderPresent(rend);
  }

  if (rend) {
      SDL_DestroyRenderer(rend);
  }
  if (win) {
      SDL_DestroyWindow(win);
  } 
  SDL_Quit();
  return 0;
}
