#include "../include/vect2.h"


float velocity_magnitude(ball* b1){
    return sqrt(b1->velX * b1->velX + b1->velY * b1->velY);
}

float vector_magnitude(vect2 v){
    return sqrt(v.x * v.x + v.y * v.y);
}

float dot_product(vect2 v1,vect2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}