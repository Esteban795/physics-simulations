#include "../include/engine.h"

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700


int min(int a,int b){
    return a < b ? a : b;
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
    if (argc != 3) return EXIT_FAILURE;
    const int nb_x = min(atoi(argv[1]),150);
    const int nb_y = min(atoi(argv[2]),150);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,SCREEN_WIDTH,SCREEN_HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;
    main_loop(renderer,nb_x,nb_y);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

//gcc cloth_simulation.c engine.c cloth.c mouse.c particle.c vect2.c -o cs -W -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm