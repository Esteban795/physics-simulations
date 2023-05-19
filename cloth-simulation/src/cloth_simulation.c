#include "../include/engine.h"

// Just an utility function used almost everywhere I use SDL2 lol
int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}


int main(int argc, char* argv[]){
    if (argc != 3) return EXIT_FAILURE;
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,1200,700,"2D Cloth simulation, by Esteban795 (on Github).");
    if (status == 1) return EXIT_FAILURE;
    main_loop(renderer,rows,columns);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}