#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/ball.h"
#include "../include/engine.h"


int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}



int main(void){
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;

    main_loop(renderer);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

//gcc ball_under_gravity.c -o bug -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs) -lSDL2