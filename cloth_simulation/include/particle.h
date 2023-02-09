#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdbool.h>



#include "vect2.h"
#include "stick.h"
#include "mouse.h"


struct Particle {
    stick sticks[2];
    float x;
    float y;
    float prevx;
    float prevy;
    float initx;
    float inity;
    float mass;
    bool is_pinned;
    bool is_selected;
};

typedef struct Particle particle;

float dist(particle p1,particle p2);

vect2 get_diff(particle* p1,particle* p2);

void print_particle(particle p);

particle** create_particles(int startX,int startY,int width,int height,int spacing,vect2* dimensions);

void update_particles(particle* particles,int n);
#endif