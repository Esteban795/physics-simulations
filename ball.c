#include <stdlib.h>
#include <math.h>

#define WIDTH 600
#define HEIGHT 600

#define BALL_BOUCINESS 0.6
#define WALL_BOUCINESS 0.8

#define GRAVITY 0.2

#define MOMENTUM_LOST 0.002

struct Ball {
    float x;
    float y;
    float velX;
    float velY;
};

typedef struct Ball ball;



ball* create_ball(float x,float y,float velX,float velY){
    ball* b = malloc(sizeof(ball));
    b->x = x;
    b->y = y;
    b->velX = velX;
    b->velY = velY;
    return b;
}

void copy_ball(ball* source,ball* dest){
    dest->x = source->x;
    dest->y = source->y;
    dest->velX = source->velX;
    dest->velY = source->velY;
}

void update_ball(ball* b,float Xgrav, float Ygrav){
    b->x += b->velX;
    b->y += b->velY;

    b->velX *= (1 - MOMENTUM_LOST);
    b->velY *= (1 - MOMENTUM_LOST);
    if (b->x < 0 || b->x > WIDTH) b->velX *= -WALL_BOUCINESS;
    if (b->y < 0 || b->y > HEIGHT) b->velY *= -WALL_BOUCINESS;

    //Action of gravity source
    float delta_x = b->x - Xgrav;
    float delta_y = b->y - Ygrav;
    float hypothenuse = sqrt(delta_x * delta_x + delta_y * delta_y);
    b->velX = GRAVITY * delta_x/hypothenuse;
    b->velY = GRAVITY * delta_y/hypothenuse;
}

void collision_handling(ball* b1, ball* b2,int bounce_direction){
    b1->velX = b2->velY;
    b1->velY = b2->velX;
    switch (bounce_direction) {
        case 0:
            b1->x--;
            break;
        case 1:
            b1->y++;
        case 2:
            b1->x++;
        case 3:
            b1->y--;
        default:
            break;
    }
}