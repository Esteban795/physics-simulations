#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

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

double vector_magnitude(vect2 v){
    return sqrtf(v.x * v.x + v.y * v.y);
}

double dot_product(vect2 v1,vect2 v2){
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
    double factor;
};

struct Point {
    vect2 pos;
    vect2 vel;
    vect2 force;
    double mass;
    bool is_fixed;
};

struct Ball {
    point* points;
    spring* springs;
    int len;
};



struct Line {
    vect2 pos;
    vect2 dir;
    bool facing;
};

typedef struct Ball ball;
typedef struct Line line;
typedef struct Spring spring;
typedef struct Point point;

void apply_force(point* p,vect2 force){
    p->force.x += force.x;
    p->force.y += force.y;
}

void update_spring(spring* s){
    vect2 difference = diff(s->p1->pos,s->p2->pos);
    float length = vector_magnitude(difference);
    float force = (length - s->length) * s->stiffness;
    vect2 normalized_diff = normalize(difference);

    vect2 velocity_diff = diff(s->p1->vel,s->p2->vel);
    float dot = dot_product(normalized_diff,velocity_diff);
    force += dot * s->damping;

    vect2 final_force = {.x = force * normalized_diff.x, .y = force * normalized_diff.y};
    if (!s->p1->is_fixed){
        apply_force(s->p1,final_force);
    }
    if (!s->p2->is_fixed){
        apply_force(s->p2,final_force);
    }
    s->factor = length / s->length;
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
       
        vect2 normal_vect = {.x = springs[i].p1->pos.y - springs[i].p2->pos.y, .y = - (springs[i].p1->pos.x - springs[i].p2->pos.x)};
    
        spring opposite_spring = springs[(i + len_springs / 2) % len_springs];
        vect2 opposite_midpoint = {.x = (opposite_spring.p1->pos.x + opposite_spring.p2->pos.x) / 2.0, .y = (opposite_spring.p1->pos.y + opposite_spring.p2->pos.y) / 2.0};
        vect2 midpoint = {.x = (springs[i].p1->pos.x + springs[i].p2->pos.x) / 2.0, .y = (springs[i].p1->pos.y + springs[i].p2->pos.y) / 2.0};
        vect2 outward_vect = normalize(diff(midpoint,opposite_midpoint));

        vect2 final_normal;
        //Making sure we flip them outwards
        if (dot(normal_vect,outward_vect) > 0) final_normal = normal_vect;
        else final_normal = multiply(normal_vect,-1.0);

        apply_force(&springs[i].p1->force,multiply(final_normal,force));
        apply_force(&springs[i].p2->force,multiply(final_normal,force));
    }
}

vect2 get_closest(line l,point p){
    vect2 v = diff(p.pos,l.dir);
    double d = dot_product(v,l.dir);
    vect2 final = {.x = l.pos.x + l.dir.x * d,.y = l.pos.y + l.dir.y * d};
    return final;
}

vect2 get_normal(line l){
    vect2 normal_direction = {.x = l.dir.y, .y = - l.dir.x};
    if (l.facing) normal_direction = multiply(normal_direction,-1.0);
    return multiply(normal_direction,0.5);
}

bool is_passing(line l,point p){
    vect2 normalized_difference  = normalize(diff(p.pos,l.pos));
    vect2 line_normal = get_normal(l);

    double dot = dot_product(normalized_difference,line_normal);
    return (dot > 0.0) != l.facing;
}

point create_point(float angle,float radius,vect2 center){
    vect2 pos = {.x = center.x + radius * cosf(angle), .y = center.y + radius * sinf(angle)};
    vect2 vel = {.x = 0.0f, .y = 0.0f};
    vect2 force = {.x = 0.0f, .y = 0.0f};
    point p = {.pos = pos, .vel = vel, .force = force,.prev_pos = pos,.mass = MASS};
    return p;
}

point* build_sphere(int nb_points,float radius,vect2 center){
    float spacing  = 3.14 * 2 / nb_points;
    point* points = malloc(sizeof(point) * nb_points);
    for (int i = 0; i < nb_points;i++){
        float angle = spacing * i;
        points[i] = create_point(angle,radius,center);
    }
    return points;
}


ball* create_circle(int radius,int nb_points){
    ball* b = malloc(sizeof(ball));
    vect2 center = {.x = WIDTH / 2, .y = HEIGHT / 2}
    b->len = nb_points;
    point* points = malloc(sizeof(nb_points));
    spring* springs = malloc(sizeof(nb_points));
}


