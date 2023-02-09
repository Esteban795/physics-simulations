#ifndef STICK_H
#define STICK_H

#include "particle.h"

struct Stick {
    particle* p1;
    particle* p2;
    float length;
    bool is_active;
    bool is_selected;
    bool is_null;
};

typedef struct Stick stick;

void create_sticks(particle** particles,int rows,int columns,int* nb_sticks);


void update_sticks(stick* sticks,int n);
#endif