#ifndef BALL_H
#define BALL_H
#include <stdlib.h>

#define BALL_RADIUS 30


struct Ball {
    float x;
    float y;
    float velX;
    float velY;
    int radius;
};

typedef struct Ball ball;



ball* create_ball(float x,float y,float velX,float velY,int radius);


void copy_ball(ball* source,ball* dest);


#endif