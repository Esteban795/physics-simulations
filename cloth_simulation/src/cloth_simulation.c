#include "../include/engine.h"

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700
#define SPACING 20


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
    if (argc != 2) return EXIT_FAILURE;
    const int WIDTH = min(argv[1],600);
    const int HEIGHT = min(argv[1],600);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,SCREEN_WIDTH,SCREEN_HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;
}

//gcc cloth_simulation.c -o st -W -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm