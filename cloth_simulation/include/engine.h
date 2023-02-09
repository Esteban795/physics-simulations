#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include "cloth.h"
#define SPACING 100


struct Application {
    cloth* c;
    SDL_Renderer* renderer;
    mouse* m;
    uint32_t last_update_time;
};

typedef struct Application application;

application* new_app(SDL_Renderer* renderer,int width,int height,int startX, int startY);

void update(application* app);

void render(application* app);

void destroy(application* app);

int main_loop(SDL_Renderer* renderer,int width,int height);
#endif