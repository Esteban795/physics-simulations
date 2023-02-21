#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


const double NRT = 1.0 * 8.31 * 293.15;


struct vect2 {
    float x;
    float y;
};

typedef struct vect2 vect2;

vect2 zero_vect = {.x = 0.0f,.y = 0.0f};

vect2 diff(vect2 v1,vect2 v2){
    vect2 temp = {.x = v1.x - v2.x,.y = v1.y - v2.y};
    return temp;
}

vect2 divide(vect2 v,float n){
    vect2 temp = {.x = v.x / n,.y = v.y / n};
    return temp;
}

float vector_magnitude(vect2 v){
    return sqrtf(v.x * v.x + v.y * v.y);
}

float dot_product(vect2 v1,vect2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}

vect2 add_vect2(vect2 v1,vect2 v2){
    vect2 sum = {.x = v1.x + v2.x,.y = v1.y + v2.y};
    return sum;
}

vect2 substract_vect2(vect2 v1, vect2 v2){
    vect2 difference = {.x = v1.x - v2.x,.y = v1.y - v2.y};
    return difference;
}

vect2 multiply(vect2 v,float n){
    vect2 product = {.x = v.x * n,.y = v.y * n};
    return product;
}

vect2 normalize(vect2 v){
    float magnitude = vector_magnitude(v);
    if (magnitude < 1e-5) return zero_vect;
    vect2 normalized = {.x = v.x / magnitude,.y = v.y / magnitude};
    return normalized;
}

const vect2 GRAVITY = {.x = 0.0,.y = 9.8};

///////////////////:

struct Spring;
struct point;

struct Spring {
    struct Point* p1;
    struct Point* p2;
    double length;
    double stiffness;
    double damping;
};

struct Point {
    vect2 pos;
    vect2 vel;
    vect2 force;
    double mass;
};

typedef struct Spring spring;
typedef struct Point point;



void update_spring(spring* s){
    vect2 difference = diff(s->p1->pos,s->p2->pos);
    float length = vector_magnitude(difference);
    float force = (length - s->length) * s->stiffness;
    vect2 normalized_diff = normalize(difference);

    vect2 velocity_diff = diff(s->p1->vel,s->p2->vel);
    float dot = dot_product(normalized_diff,velocity_diff);
    force += dot * s->damping;

    s->p1->force.x += force * (s->p1->pos.x - s->p2->force.x) / length;
    s->p1->force.y += force * (s->p1->pos.y - s->p2->force.y) / length;
    s->p2->force.x -= force * (s->p1->pos.x - s->p2->force.x) / length;
    s->p2->force.y -= force * (s->p1->pos.y - s->p2->force.y) / length;
}


double calculate_area(point* points,int len_points){
    double area = 0.0;
    for (int i = 0; i < len_points;i++){
        int i_next = (i + 1) % len_points;
        area += points[i].pos.y * points[i_next].pos.x - points[i].pos.x * points[i_next].pos.y;
    }
    return area;
}

void apply_pressure(point* points,spring* springs,int len_springs){
    double area =  calculate_area(points,len_springs);
    for (int i = 0; i < len_springs;i++){
        double dist = vector_magnitude(diff(springs[i].p1->pos,springs[i].p2->pos));
        double force =  (dist * NRT) / area;

        /*
        Gotta calculate normal vectors
        */
       
    }
}

void apply_force(point* p,vect2 force){
    p->force.x += force.x;
    p->force.y += force.y;
}