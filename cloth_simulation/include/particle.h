#ifndef PARTICLE_H
#define PARTICLE_H

#include "vect2.h"
struct Particle {
    float x;
    float y;
    float prevx;
    float prevy;
    float mass;
};

typedef struct Particle particle;

float dist(particle p1,particle p2);

vect2 get_diff(particle* p1,particle* p2);

void print_particle(particle p);

particle* create_particles(int n);

void update_particles(particle* particles,int n);
#endif