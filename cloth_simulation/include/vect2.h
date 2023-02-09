#ifndef VECT2_H
#define VECT2_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct vect2 {
    float x;
    float y;
};

typedef struct vect2 vect2;

vect2 diff(vect2 v1,vect2 v2);

vect2 divide(vect2 v,float n);

float vector_magnitude(vect2 v);

float dot_product(vect2 v1,vect2 v2);

float get_length(vect2 p);

#endif