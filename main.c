#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "ball.c"


#define NB_BALLS 256
#define FRAME_DELAY 17


float rand_between(int l, int r) {
  return ( (rand() / (RAND_MAX * 1.0f)) * (r - l) + l);
}



int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}


int main(int argc,char* argv[]){
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;

    int running = 1;
    SDL_Event e;
    int mouse_x,mouse_y,prev_mouse_x,prev_mouse_y;
    ball* mouse_ball = create_ball(0,0,0,0);

    ball** balls = malloc(NB_BALLS * sizeof(ball*));

    for (int i = 0; i < NB_BALLS;i++){
        float x = rand_between(10,WIDTH - 10);
        float y = rand_between(10,HEIGHT - 10);
        balls[i] = create_ball(x,y,0,0);
    }

    ball** prev_frame_balls = malloc(NB_BALLS * sizeof(ball*));

    for (int i = 0; i < NB_BALLS; i++) {
        prev_frame_balls[i] = create_particle(0, 0, 0, 0);
    }

    while (running){
        SDL_Delay(FRAME_DELAY);
        prev_mouse_x = mouse_x;
        prev_mouse_y = mouse_y;

        SDL_GetMouseState(&mouse_x,&mouse_y);
        mouse_ball->x = (float) mouse_x;
        mouse_ball->y = (float) mouse_y;
        mouse_ball->velX = (float) mouse_x - prev_mouse_x;
        mouse_ball->velY = (float) mouse_y - prev_mouse_y;

        for (int i = 0; i < NB_BALLS;i++){
            copy_ball(balls[i],prev_frame_balls[i]);
        }
        int bounce_direction = 0;
        for (int i = 0; i < NB_BALLS;i++){
            bounce_direction %= 3;
            for (int j = 0; j < NB_BALLS;j++){
                bounce_direction %= 3;
                if (i = j) continue;
                if 
            }
        }
    }
}