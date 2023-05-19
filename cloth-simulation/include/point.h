#ifndef POINT_H
#define POINT_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "vect2.h"
#include "mouse.h"

#define SPACING 10


/*
Why is the struct stick here you might ask ? Because Stick needs Point and Point needs Stick, so that was the only work around that I found.
*/

struct Stick;
struct Point;

struct Stick {
    struct Point* p1;
    struct Point* p2;
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


void print_point(point p);

point** create_points(int startX,int startY,int rows,int columns,int spacing);

void keep_inside_view(point* p,int width,int height);

void update_point(point* p,float dt,float drag, vect2 acceleration,float elasticity,mouse* m,const int SCREEN_WIDTH,const int SCREEN_HEIGHT);


#endif