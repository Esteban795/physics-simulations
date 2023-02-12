#ifndef CLOTH_h
#define CLOTH_H

#include "point.h"
#include "stick.h"
struct Cloth {
    vect2 acceleration;
    float drag;
    float elasticity;
    point** particles;
    int rows;
    int columns;
    stick** sticks;
    int nb_sticks;
};

typedef struct Cloth cloth;

cloth* cloth_new(float drag,float elasticity,int rows,int columns);

void cloth_update(cloth* c,mouse* m);

void cloth_draw(SDL_Renderer* renderer,cloth* c);

void cloth_delete(cloth* c);


#endif 