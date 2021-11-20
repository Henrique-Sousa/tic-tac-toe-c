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

int makeMove(enum Player player, int board[N][N], int x, int y) {
  int i = y / CELL_HEIGHT;
  int j = x / CELL_WIDTH;
  if (board[i][j] == _) {
    board[i][j] = player;
    return 1;
  }
  return 0;
}

void switchPlayer(enum Player* player) {
  if (*player == X) {
    *player = O;
  } else if (*player == O) {
    *player = X;
  }
}

int checkWinner(int board[N][N]) {
  enum Player player; 
  // verify rows
  for (int i = 0; i < N; i++) {
    player = board[i][0];
    if (player == _) {
      continue;
    }
    int j = 1;
    while (j < N && player == board[i][j]) {
      j++;
    }
    if (j == N) {
      return player;
    }
  }
  // verify columns 
  for (int j = 0; j < N; j++) {
    player = board[0][j];
    if (player == _) {
      continue;
    }
    int i = 1;
    while (i < N && player == board[i][j]) {
      i++;
    }
    if (i == N) {
      return player;
    }
  }
  // verify main diagonal
  player = board[0][0];
  if (player != _) {
    int i = 1;
    while (i < N && player == board[i][i]) {
      i++;
    }
    if (i == N) {
      return player;
    }
  }
  // verify secondary diagonal
  player = board[0][N-1];
  if (player != _) {
    int i = 1;
    while (i < N && player == board[i][N-i-1]) {
      i++;
    }
    if (i == N) {
      return player;
    }
  }

  return 0;
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
  int moveMade = 0;
  int winner = 0;
  while (quit == 0) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT:
          quit = 1;
          break;
        case SDL_MOUSEBUTTONDOWN:
          moveMade = makeMove(player, board, event.button.x, event.button.y);
          winner = checkWinner(board);
          if (winner) {
            printf("the winner is %d", winner);
            quit = 1;
          } else if (moveMade) {
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
