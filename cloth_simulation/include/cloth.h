#ifndef CLOTH_H
#define CLOTH_H

#include "vect2.h"
#include "particle.h"
#include "stick.h"

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


#endif