#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#define WIDTH 900
#define HEIGHT 900
#define MASS 20.0
#define DAMPING 0.9


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
struct Point;

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
    struct Point* points;
    struct Spring* springs;
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
        if (dot_product(normal_vect,outward_vect) > 0) final_normal = normal_vect;
        else final_normal = multiply(normal_vect,-1.0);

        apply_force(springs[i].p1,multiply(final_normal,force));
        apply_force(springs[i].p2,multiply(final_normal,force));
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
    point p = {.pos = pos, .vel = vel, .force = force,.mass = MASS};
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

spring create_spring(point* p1,point* p2){
    double dist = vector_magnitude(diff(p1->pos,p2->pos));
    spring s = {.p1 = p1, .p2 = p2, .damping = DAMPING, .stiffness = 200.0, .length = dist};
    return s;
}

spring* build_springs(point* points,int nb_points){
    spring* springs = malloc(sizeof(spring) * nb_points);
    for (int i = 0; i < nb_points;i++){
        int next = (i + 1) % nb_points;
        springs[i] = create_spring(&points[i],&points[next]);
    }
    return springs;
}

ball* create_circle(int radius,int nb_points){
    ball* b = malloc(sizeof(ball));
    vect2 center = {.x = WIDTH / 2, .y = HEIGHT / 2};
    b->len = nb_points;
    b->points = build_sphere(nb_points,radius,center);
    b->springs = build_springs(b->points,nb_points);
    return b;
}

void ball_free(ball* b){
    free(b->points);
    free(b->springs);
    free(b);
}

int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}

void draw_spring(SDL_Renderer* renderer,spring s){
    SDL_RenderDrawLine(renderer,s.p1->pos.x,s.p1->pos.y,s.p2->pos.x,s.p2->pos.y);
}

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius){
   const int32_t diameter = (radius * 2);
   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);
   while (x >= y){
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);
      if (error <= 0){
        ++y;
        error += ty;
        ty += 2;
      }
      if (error > 0){
        --x;
        tx += 2;
        error += (tx - diameter);
      }
   }
}


void ball_draw(SDL_Renderer* renderer,ball* b){
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    for (int i = 0; i < b->len;i++){
        DrawCircle(renderer,b->points[i].pos.x,b->points[i].pos.y,50);
        draw_spring(renderer,b->springs[i]);
    }
    SDL_RenderPresent(renderer);
}

int main(int argc,char* argv[]){
    if (argc != 2) return EXIT_FAILURE;
    int nb_points = atoi(argv[1]);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"softball test");
    if (status == 1) return EXIT_FAILURE;
    ball* b = create_circle(300,nb_points);
    ball_draw(renderer,b);
    SDL_Delay(4000);
    ball_free(b);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

//gcc soft_ball_test.c -o sbt -Wall -Wvla -Wextra -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm