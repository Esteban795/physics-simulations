#ifndef STICK_H
#define STICK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "point.h"


void print_stick(stick s);

int get_nb_sticks(int rows,int columns);

stick* create_new_stick(point* p1,point* p2);

stick** create_sticks(point** points,int rows,int columns,int* nb_sticks);

void update_stick(stick* s);

void draw_stick(SDL_Renderer* renderer,stick* s);


#endif 