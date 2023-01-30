#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>  

#define WIDTH 600
#define HEIGHT 600
#define FRAME_DELAY 17

#define GRAVITY 1


int is_distance(int x_pos_0,int y_pos_0,int x_pos_1,int y_pos_1,float distance){
    int horiz_dist = x_pos_1 - x_pos_0;
    int vert_dist = y_pos_1 - y_pos_0;
    float points_dist = (
        pow((float)horiz_dist, 2.0)
        + pow((float)vert_dist, 2.0)
    );

    return (points_dist <= sqrt(distance));
}


struct Ball {
    float x;
    float y;
    float velX;
    float velY;
    int radius;
};

typedef struct Ball ball;

ball* create_ball(float x,float y,float velX,float velY,int radius){
    ball* b = malloc(sizeof(ball));
    b->x = x;
    b->y = y;
    b->velX = velX;
    b->velY = velY;
    b->radius = radius;
    return b;
}

void copy_ball(ball* source,ball* dest){
    dest->x = source->x;
    dest->y = source->y;
    dest->velX = source->velX;
    dest->velY = source->velY;
    dest->radius = source->radius;
}

float rand_between(int l, int r) {
  return ( (rand() / (RAND_MAX * 1.0f)) * (r - l) + l);
}

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius){
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

int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}

void update_ball(ball* b){
    b->x += b->velX;
    //b->y += b->velY;
    //b->velY += GRAVITY;
}


bool checkCollision(ball* b){
    if (b->y + b->radius > HEIGHT){
        b->y = HEIGHT - b->radius;
        return true;
    }
    return false;
}

bool check_wall_collision(ball* b){
    if (b->x + b->radius > WIDTH){
        b->x = WIDTH - b->radius;
        return true;
    } else if (b->x - b->radius < 0){
        b->x = b->radius;
        return true;
    }
    return false;
}

float abs_float(float n){
    return n > 0 ? n:-n;
}


bool check_ball_collision(ball* b1,ball* b2){
    return abs_float(b1->x - b2->x) < b1->radius + b2->radius;
}

int main(void){
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;

    int running = 1;
    SDL_Event e;
    ball* b1 = create_ball(WIDTH/4,HEIGHT/2,10,0,30);
    ball* b2 = create_ball(3 * WIDTH/4,HEIGHT/2,-10,0,30);

    ball* prev_b1 = create_ball(WIDTH/4,HEIGHT/2,10,0,30);
    ball* prev_b2 = create_ball(3 * WIDTH/4,HEIGHT/2,-10,0,30);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    DrawCircle(renderer,b1->x,b1->y,b1->radius);
    SDL_SetRenderDrawColor(renderer,0,0,255,255);
    DrawCircle(renderer,b2->x,b2->y,b2->radius);
    SDL_RenderPresent(renderer);

    while (running){
        SDL_Delay(FRAME_DELAY);

        copy_ball(b1,prev_b1);
        copy_ball(b2,prev_b2);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);

        update_ball(b1);
        update_ball(b2);
        if (check_wall_collision(b1)) b1->velX *= -1;
        if (check_wall_collision(b2)) b2->velX *= -1;
        if (check_ball_collision(b1,b2)){
            b1->velX *= -1;
            b2->velX *= -1;
        }

        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        DrawCircle(renderer,(int)b1->x,(int)b1->y,b1->radius);
        SDL_SetRenderDrawColor(renderer,0,0,255,255);
        DrawCircle(renderer,(int)b2->x,(int)b2->y,b2->radius);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym)
                {
                case SDLK_q:
                    running = 0;
                    break;
                case SDLK_1:
                    b1->velX = rand_between(10,30) - 20;
                    b1->velY = -rand_between(20,60);
                    b2->velX = rand_between(10,30) - 20;
                    b2->velY = -rand_between(20,60);
                default:
                    break;
                }
            }
        }
    }
    free(b1);
    free(b2);
    free(prev_b1);
    free(prev_b2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

//gcc collision_test.c -o ct -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs) -lSDL2 -lm
