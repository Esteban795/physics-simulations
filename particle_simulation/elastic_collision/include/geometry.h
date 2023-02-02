#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdlib.h>


int is_distance(int x_pos_0,int y_pos_0,int x_pos_1,int y_pos_1,float distance);

float rand_between(int l, int r);

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius);

float abs_float(float n);

#endif