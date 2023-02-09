#include "../include/vect2.h"

float vector_magnitude(vect2 v){
    return sqrt(v.x * v.x + v.y * v.y);
}

float dot_product(vect2 v1,vect2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}

float get_length(vect2 p){
    return sqrt(p.x * p.x + p.y * p.y);
}