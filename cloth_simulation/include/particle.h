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



struct Particle {
    struct Stick* sticks;
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

typedef struct Stick stick;
typedef struct Particle particle;


stick* create_sticks(particle** particles,int rows,int columns,int* nb_sticks);

void draw_stick(SDL_Renderer* renderer,stick s);

void update_stick(stick* s);

float dist(particle p1,particle p2);

vect2 get_diff(particle* p1,particle* p2);

void print_particle(particle p);

particle** create_particles(int startX,int startY,int width,int height,int spacing);

void update_particle(particle* p,float dt,float drag, vect2 acceleration,float elasticity,mouse* m,int width,int height);
#endif