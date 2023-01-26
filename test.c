#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>


#define WIDTH 600
#define HEIGHT 600

int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}


void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius){
   const int32_t diameter = (radius * 2);
   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);
   while (x >= y){
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);
      if (error <= 0){
        ++y;
        error += ty;
        ty += 2;
      }
      if (error > 0){
        --x;
        tx += 2;
        error += (tx - diameter);
      }
   }
}
int rand_between(int l, int r) {
  return (int)( (rand() / (RAND_MAX * 1.0f)) * (r - l) + l);
}


struct Ball {
  int x;
  int y;
  int velX;
  int velY;
  int radius;
};

typedef struct Ball ball;

bool check_x_bound(ball b){
  return (b.x - b.radius) > 0 && (b.x + b.radius < WIDTH);
}

bool check_y_bound(ball b){
  return (b.y - b.radius > 0) && (b.y + b.radius < HEIGHT);
}

void apply_velocity(ball* b){
  if (!check_x_bound(*b)){
    b->velX *= -1;
  }
  if (!check_y_bound(*b)){
    b->velY *= -1;
  }
  b->x += b->velX;
  b->y += b->velY;
}

int main(void){
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    int x = WIDTH/2;
    int y = HEIGHT/2;
    DrawCircle(renderer,x,y,40);
    int velX = rand_between(1,10);
    int velY = rand_between(1,10);
    ball b = {.x=x,.y = y,.velX=velX,.velY=velY,.radius = 30};
    while (true){
      SDL_SetRenderDrawColor(renderer,0,0,0,255);
      SDL_RenderClear(renderer);
      apply_velocity(&b);
      SDL_SetRenderDrawColor(renderer,255,255,255,255);
      DrawCircle(renderer,b.x,b.y,b.radius);
      SDL_RenderPresent(renderer);
      SDL_Delay(7);
    }
    SDL_Delay(5000);
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if (window != NULL) SDL_DestroyWindow(window);
    return 0;
}


//gcc test.c -o -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs) -lSDL2