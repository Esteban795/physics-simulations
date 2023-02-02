#ifndef VECT2_H
#define VECT2_H

#include "../include/ball.h"
#include <math.h>

struct vect2 {
    float x;
    float y;
};

typedef struct vect2 vect2;

float velocity_magnitude(ball* b1);

float vector_magnitude(vect2 v);

float dot_product(vect2 v1,vect2 v2);

#endif