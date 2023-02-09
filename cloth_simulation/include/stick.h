#ifndef STICK_H
#define STICK_H

#include "particle.h"

struct Stick {
    particle* p1;
    particle* p2;
    float length;
};

typedef struct Stick stick;

stick* create_sticks(particle* particles,int n);

void update_sticks(stick* sticks,int n);
#endif