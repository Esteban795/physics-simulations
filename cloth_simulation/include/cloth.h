#ifndef CLOTH_H
#define CLOTH_H

#include "vect2.h"
#include "particle.h"


struct Cloth {
    vect2 gravity;
    float drag;
    float elasticity;
    particle** particles;
    stick* sticks;
    int rows;
    int columns;
    int nb_sticks;
};

typedef struct Cloth cloth;

void cloth_draw(cloth* c,SDL_Renderer* renderer);

void cloth_update(cloth* c,mouse* m,float dt);

cloth* new_cloth(int width,int height, int spacing, int startX,int startY);

void cloth_delete(cloth* c);
#endif