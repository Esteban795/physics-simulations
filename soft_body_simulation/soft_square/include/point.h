#ifndef POINT_H
#define POINT_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "vect2.h"

#define POINT_MASS 10.0

struct Spring;
struct Point;

struct Spring {
    struct Point* p1;
    struct Point* p2;
    double length;
    double stiffness;
    double damping;
};


//Even though we use verlet integration, we keep track of points' velocities to apply dampering forces
struct Point { 
    vect2 pos;
    vect2 vel;
    vect2 prev_pos;
    vect2 force;
    double mass;
    bool is_fixed;
};

typedef struct Spring spring;
typedef struct Point point;

point** create_points(int startX,int startY,int rows,int columns,int spacing);

void update_point(point* p,double dt);

void apply_force(point* p,vect2 f);

void update_positions(point** points,double dt,int rows, int columns);

void update_velocities(point** points,double dt,int rows, int columns);

void clear_forces(point** points,int rows,int columns);

void delete_points(point** points,int rows);

void keep_inside_view(point* p,int width,int height);
#endif