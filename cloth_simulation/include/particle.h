#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdbool.h>


#include <SDL2/SDL.h>
#include "vect2.h"
#include "mouse.h"

struct Stick;
struct Particle;

struct Stick {
    struct Particle* p1;
    struct Particle* p2;
    float length;
    bool is_active;
    bool is_selected;
    bool is_null;
};

typedef struct Stick stick;

struct Particle {
    stick* sticks;
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


stick* create_sticks(particle** particles,int rows,int columns,int* nb_sticks);

void update_sticks(stick* sticks,int n);

float dist(particle p1,particle p2);

vect2 get_diff(particle* p1,particle* p2);

void print_particle(particle p);

particle** create_particles(int startX,int startY,int width,int height,int spacing);

void update_particles(particle* particles,int n);
#endif