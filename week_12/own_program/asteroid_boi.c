#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>

struct sprite {
  int vel;
  bool alive;
  SDL_Rect *rect;
};
typedef struct sprite sprite;

struct asteroid {
  int vel;
  SDL_Rect *rect;
};
typedef struct asteroid asteroid;

struct game {
  SDL_Renderer *ren;
  SDL_Window *window;
  sprite *ship;
  asteroid *ast[3];
};
typedef struct game game;

SDL_Rect *create_rect(int x, int y, int w, int h) {
  SDL_Rect *rect = malloc(sizeof(SDL_Rect));
  rect -> x = x;
  rect -> y = y;
  rect -> w = w;
  rect -> h = h;
  return rect;
}

sprite *create_sprite(SDL_Rect *r) {
  sprite *s = malloc(sizeof(sprite));
  s -> rect = r;
  s -> alive = true;
  s -> vel = 8;
  return s;
}

asteroid *create_asteroid(SDL_Rect *r) {
  asteroid *a = malloc(sizeof(asteroid));
  a -> rect = r;
  a -> vel = 1;
  return a;
}

game *initialise() {
  game *game = malloc(sizeof(game));
  game -> window = SDL_CreateWindow("Asteroid Boi",
                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                  400, 400,
                  0);
  game -> ren = SDL_CreateRenderer(game -> window, -1, 0);
  game -> ship = create_sprite(create_rect(200, 300,
                 30, 30));
  for (int i = 0; i < 51; i++) {
    int ranx = rand()%400;
    int rany = rand()%400;
    rany *= -1;
    int ranw = rand()%30 + 20;
    int ranh = ranw;
    game -> ast[i] = create_asteroid(create_rect(ranx, rany, ranw, ranh));
  }
  srand(time(NULL));
  return game;
}

void free_game(game *g) {
  free(g -> window);
  free(g -> ren);
  free(g -> ship);
  for (int i = 0; i < 4; i++) {
    free(g -> ast[i]);
  }
}

void key_press(sprite *s) {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        printf("");
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT]) s -> rect -> x-= s -> vel;
        if (state[SDL_SCANCODE_RIGHT]) s -> rect -> x+= s -> vel;
        if (state[SDL_SCANCODE_UP]) s -> rect -> y-= s -> vel;
        if (state[SDL_SCANCODE_DOWN]) s -> rect -> y+= s -> vel;
        break;
      case SDL_QUIT:
        s -> alive = false;
        break;
      default:
        break;
    }
  }
}

void update_pos(asteroid *a) {
  int i = rand()%50;
  if(i == 25) {
    if (a -> rect -> y > 400) {
      int random = rand()%450;
      random *= -1;
      a -> rect -> y = random;
      random = rand()%350;
      a -> rect -> x = random;
      random = rand()%30 + 20;
      a -> rect -> w = random;
      a -> rect -> h = random;
    }
    else {
      a -> rect -> y += a -> vel;
    }
  }
}

void border_collision(game *g) {
  int ship_x = g -> ship -> rect -> x;
  int ship_y = g -> ship -> rect -> y;
  int ship_w = g -> ship -> rect -> w;
  int ship_h = g -> ship -> rect -> h;
  if (ship_x < 0 | ship_x + ship_w > 400 | ship_y < 0 | ship_y + ship_h > 400) {
    printf("You died!!!\n");
    g -> ship -> alive = false;
  }
}

void asteroid_collision(game *g) {
  int ship_x = g -> ship -> rect -> x;
  int ship_y = g -> ship -> rect -> y;
  int ship_w = g -> ship -> rect -> w;
  int ship_h = g -> ship -> rect -> h;
  for (int i = 0; i < 4; i++) {
    if (ship_x < g -> ast[i] -> rect -> x + g -> ast[i] -> rect -> w &
        ship_x + ship_w > g -> ast[i] -> rect -> x &
        ship_y < g -> ast[i] -> rect -> y + g -> ast[i] -> rect -> h &
        ship_y + ship_h > g -> ast[i] -> rect -> y) {
          printf("You died!!!\n");
          g -> ship -> alive = false;
        }
  }
}

void re_draw(game *g) {
  SDL_SetRenderDrawColor(g -> ren, 0, 0, 0, 255);
  SDL_RenderClear(g -> ren);
  SDL_SetRenderDrawColor(g -> ren, 255, 255, 255, 255);
  SDL_RenderFillRect(g -> ren, g -> ship -> rect);
  SDL_SetRenderDrawColor(g -> ren, 255, 0, 0, 255);
  for (int i = 0; i < 4; i++) {
    SDL_RenderFillRect(g -> ren, g -> ast[i] -> rect);
  }
  SDL_RenderPresent(g -> ren);
}

void run_game() {
  game *g = initialise();
  while(g -> ship -> alive == true) {
    border_collision(g);
    asteroid_collision(g);
    for (int i = 0; i < 4; i++) update_pos(g -> ast[i]);
    key_press(g -> ship);
    re_draw(g);
  }
  free_game(g);
}

int main(int n, char *args[]) {
  run_game();
  return 0;
}
