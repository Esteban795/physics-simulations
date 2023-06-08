#include "../include/vect2.h"

vect2 vect2_diff(vect2 v1,vect2 v2){
    vect2 temp = {.x = v1.x - v2.x,.y = v1.y - v2.y};
    return temp;
}

vect2 vect2_divide(vect2 v,double n) {
    vect2 temp = {.x = v.x / n,.y = v.y / n};
    return temp;
}

double vect2_length(vect2 v){
    return sqrt(v.x * v.x + v.y * v.y);
}

double dot_product(vect2 v1,vect2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}

vect2 vect2_add(vect2 v1, vect2 v2){
    vect2 temp = {.x = v1.x + v2.x, .y = v1.y + v2.y};
    return temp;
}

vect2 vect2_multiply(vect2 v1, double n){
    vect2 temp = {.x = v1.x * n, .y = v1.y * n};
    return temp;
}