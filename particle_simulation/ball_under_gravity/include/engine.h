#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ball.h"


#define WIDTH 600
#define HEIGHT 600


#define FRAME_DELAY 17
#define GRAVITY 1
#define GROUND_BOUNCINESS 0.5
#define WALL_BOUNCINESS 0.9
#define GROUND_FRICTION 0.9


float rand_between(int l, int r);

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius);

bool check_x_bound(ball b);

bool check_y_bound(ball b);

bool check_ground_collision(ball* b);

bool check_wall_collision(ball* b);

float abs_float(float n);

void update_ball(ball* b);

void main_loop(SDL_Renderer* renderer);

#endif