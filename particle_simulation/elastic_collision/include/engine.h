#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>  



#include "vect2.h"
#include "geometry.h"


#define WIDTH 1500
#define HEIGHT 900
#define FRAME_DELAY 6

#define GRAVITY 0.1
#define WALL_BOUNCINESS 0.5
#define BALL_BOUNCINESS 0.9
#define GROUND_FRICTION 0.9
#define GROUND_BOUNCINESS 0.9



void update_ball(ball* b);

bool check_ground_collision(ball* b);

bool check_wall_collision(ball* b);

bool check_ball_x_collision(ball* b1,ball* b2);

bool check_ball_y_collision(ball* b1,ball* b2);

void do_elastic_collision(ball* b1,ball* b2);

void main_loop(SDL_Renderer* renderer,int NB_BALLS);

#endif 