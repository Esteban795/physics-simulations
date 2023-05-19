#ifndef ENGINE_H
#define ENGINE_H


#include <SDL2/SDL.h>
#include "cloth.h"


struct Application {
    SDL_Renderer* renderer;
    cloth* c;
    mouse* m;
};

typedef struct Application application;

application* application_create(SDL_Renderer* renderer,float drag,float elasticity,int rows,int columns);

void application_delete(application* app);

void application_update(application* app);

void application_render(application* app);

int main_loop(SDL_Renderer* renderer,int rows,int columns);

#endif