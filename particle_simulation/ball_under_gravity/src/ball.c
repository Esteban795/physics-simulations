#include "../include/ball.h"



ball* create_ball(float x,float y,float velX,float velY,int radius){
    ball* b = malloc(sizeof(ball));
    b->x = x;
    b->y = y;
    b->velX = velX;
    b->velY = velY;
    b->radius = radius < BALL_RADIUS ? radius : BALL_RADIUS;
    return b;
}

void copy_ball(ball* source,ball* dest){
    dest->x = source->x;
    dest->y = source->y;
    dest->velX = source->velX;
    dest->velY = source->velY;
    dest->radius = source->radius;
}