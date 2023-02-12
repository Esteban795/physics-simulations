#ifndef POINT_H
#define POINT_H

#include <SDL2/SDL.h>
#include "vect2.h"

#define SPACING 10


struct Stick;
Struct Particle;

struct Stick {
    struct Particle* p1;
    struct Particle* p2;
    float length;
    bool is_active;
    bool is_selected;
};

struct Point {
    struct Stick* s1;
    struct Stick* s2;
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
typedef struct Point point;



#endif