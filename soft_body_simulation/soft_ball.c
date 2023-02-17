#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>


#define FPS 60
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700
#define MASS 1.0
#define GRAVITY 110.0f
#define DT 0.016f

#define TDF 0.99
#define NDF 0.1
#define STIFFNESS 100
#define REST_LENGTH 20
#define DAMP 0.4
#define FINAL_PRESSURE 7

float PRESSURE = 14; //initial pressure inside the ball

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

struct Point;
struct Stick;

struct Point {
    vect2 pos;
    vect2 vel;
    vect2 force;
    vect2 prev_pos;
    float mass;
};

struct Spring {
    vect2 normal;
    int p1;
    int p2;
    float length;
    float damping;
};

typedef struct Point point;
typedef struct Spring spring;

float calculate_area(point* points,spring* springs,int nb_points){
    float area = 0.0f;
    for (int i = 0; i < nb_points;i++){
        point p1 = points[springs[i].p1];
        point p2 = points[springs[i].p2];
        vect2 pos_diff = diff(p1.pos,p2.pos);
        float dist = vector_magnitude(pos_diff);
        area = 0.5f * fabsf(pos_diff.x) * fabsf(springs[i].normal.x) * dist;
    }
    return area;
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

vect2 get_spring_normal(spring s,float dist,point points[]){
    point p1 = points[s.p1];
    point p2 = points[s.p2];
    vect2 normal = {.x = - (p1.pos.y - p2.pos.y) / dist, .y = (p1.pos.x - p2.pos.y) / dist};
    return normal;
}

spring* create_springs(int nb_points,point* points){
    spring* springs = malloc(sizeof(spring) * nb_points);
    for (int i = 0; i < nb_points;i++){
        float dist = vector_magnitude(diff(points[i].pos,points[(i + 1) % nb_points].pos));
        spring s = {.p1 = i,.p2 = (i + 1) % nb_points, .damping = DAMP,.length = dist};
        s.normal = get_spring_normal(s,dist,points);
        springs[i] = s;
    }
    return springs;
}



int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}

void keep_inside_view(point* p,int width,int height){
    if (p->pos.x > width){
        p->pos.x = width;
        p->vel.x *= -1.0f;
    }
    if (p->pos.x < 0){
        p->pos.x = 0;
        p->vel.x *= -1.0f;
    }
    if (p->pos.y > height){
        p->pos.y = height;
        p->vel.y *= -1.0f;
    }
}

void update_physics(spring* springs,point* points,int nb_points){
    //updating springs
    for (int i = 0;i < nb_points;i++){
        points[i].force.x = 0;
        if (PRESSURE >= FINAL_PRESSURE){
            printf("on a dépassé\n");
            points[i].force.y = GRAVITY * MASS;
        }
    }
    for (int i = 0; i < nb_points;i++){
        point p1 = points[springs[i].p1];
        point p2 = points[springs[i].p2];

        vect2 pos_diff = diff(p1.pos,p2.pos);
        vect2 direction = normalize(pos_diff);
        float dist = vector_magnitude(pos_diff);
        if (dist < 1e-5) continue;

        vect2 vel_diff = diff(p1.vel,p2.vel);

        float f = (dist - springs[i].length) * STIFFNESS + (vel_diff.x * pos_diff.x + vel_diff.y * pos_diff.y) * springs[i].damping / dist;
        float fx = f * direction.x;
        float fy = f * direction.y;

        points[springs[i].p1].force.x -= fx;
        points[springs[i].p1].force.y -= fy;
        points[springs[i].p2].force.x += fx;
        points[springs[i].p2].force.y += fy;

        springs[i].normal = get_spring_normal(springs[i],dist,points);
    }
    
    float area = calculate_area(points,springs,nb_points);
    for (int i = 1; i < nb_points - 1;++i){
        point p1 = points[springs[i].p1];
        point p2 = points[springs[i].p2];
        vect2 pos_diff = diff(p1.pos,p2.pos);
        float dist = vector_magnitude(pos_diff);

        float pressurev = dist * PRESSURE * 1.0f/area;

        points[springs[i].p1].force.x += springs[i].normal.x * pressurev;
        points[springs[i].p1].force.y += springs[i].normal.y * pressurev;
        points[springs[i].p2].force.x += springs[i].normal.x * pressurev;
        points[springs[i].p2].force.y += springs[i].normal.y * pressurev;
    }
}

float min(float a, float b){
    return a < b ? a : b;
}

float max(float a, float b){
    return a > b ? a : b;
}

void heun_integrator(spring springs[],point points[],int nb_points,int radius){
    float drx, dry;
    float* fxsaved = malloc(sizeof(float) * nb_points);
    float* fysaved = malloc(sizeof(float) * nb_points);
    float* vxsaved = malloc(sizeof(float) * nb_points);
    float* vysaved = malloc(sizeof(float) * nb_points);

    for (int i = 0; i < nb_points;i++){
        fxsaved[i] = points[i].force.x;
        fysaved[i] = points[i].force.y;
        vxsaved[i] = points[i].vel.x;
        vysaved[i] = points[i].vel.y;

        //Explicit euler integration
        points[i].vel.x += DT * points[i].force.x / MASS;
        drx = points[i].vel.x * DT;

        points[i].vel.y += DT * points[i].force.y / MASS;
        dry = points[i].vel.y * DT;

        points[i].pos.x += drx;
        points[i].pos.y += drx;
    }
    update_physics(springs,points,nb_points);
    
    /*
    for (int i = 0; i < nb_points;i++){
        points[i].vel.x = vxsaved[i] + (points[i].force.x + fxsaved[i]) / MASS  * DT / 2;
        drx = points[i].vel.x * DT;
        points[i].pos.x += drx;

        points[i].vel.y = vysaved[i] + (points[i].force.y + fysaved[i]) / MASS  * DT / 2;
        dry = points[i].vel.y * DT;
        points[i].pos.y += dry;
        
        //Checking boundaries 
        points[i].pos.x = min(points[i].pos.x,SCREEN_WIDTH / 2.0f + radius);
        points[i].pos.x = max(points[i].pos.x, SCREEN_WIDTH / 2.0f - radius);

        points[i].pos.y = min(points[i].pos.y,SCREEN_HEIGHT / 2.0f + radius);
        points[i].pos.y = max(points[i].pos.y, SCREEN_HEIGHT / 2.0f - radius);

        if (points[i].pos.x + drx > -sqrtf(radius * radius - pow(points[i].pos.y - SCREEN_HEIGHT / 2.0f,2)) + SCREEN_WIDTH / 2.0f
            || points[i].pos.x + drx < -sqrtf(radius * radius - pow(points[i].pos.y - SCREEN_HEIGHT / 2.0f,2)) + SCREEN_WIDTH / 2.0f) {
                drx *= -1.0f;
                dry *= -1.0f;

                float vx0 = points[i].vel.x;
                float vy0 = points[i].vel.y;

                float sinTheta = (points[i].pos.y - SCREEN_HEIGHT / 2.0f) / radius;
                float cosTheta = (points[i].pos.x - SCREEN_WIDTH / 2.0f) / radius;

                points[i].vel.x = -vx0;
                points[i].vel.y = -vy0;
                points[i].vel.x = vy0 * (-TDF * sinTheta * cosTheta - NDF * sinTheta * cosTheta) + vx0 * (TDF * sinTheta * sinTheta - NDF * cosTheta * cosTheta);
                points[i].vel.y = vy0 * (TDF * cosTheta * cosTheta - NDF * sinTheta * sinTheta) + vx0 * (-TDF * sinTheta * cosTheta - NDF * sinTheta * cosTheta);
            }
        
        if (points[i].pos.y > (SCREEN_HEIGHT + radius) / 2.0f){
            float other = fabs(radius * radius - pow(points[i].pos.x - SCREEN_WIDTH / 2.0f,2));
            points[i].pos.y = min(points[i].pos.y,sqrtf(other)  + SCREEN_HEIGHT/ 2.0f);
        }
        if (points[i].pos.y < (SCREEN_HEIGHT - radius) / 2.0f) {
            float other = fabs(radius * radius - pow(points[i].pos.x - SCREEN_WIDTH / 2.0f,2));
            points[i].pos.y = max(points[i].pos.y,-sqrtf(other)  + SCREEN_HEIGHT/ 2.0f );
        }
        if (points[i].pos.x > (SCREEN_WIDTH + radius) / 2.0f){
            float other = fabs(radius * radius - pow(points[i].pos.y - SCREEN_HEIGHT/2.0,2));
            points[i].pos.x = min(points[i].pos.x,sqrtf(other)  + SCREEN_WIDTH / 2.0f);
        }
        if (points[i].pos.x > (SCREEN_WIDTH - radius) / 2.0f){
            float other = fabs(radius * radius - pow(points[i].pos.y - SCREEN_HEIGHT / 2.0f,2));
            points[i].pos.x = max(points[i].pos.x,-sqrtf(other) + SCREEN_WIDTH/ 2.0f);
        }
    }
    */
    free(fxsaved);
    free(fysaved);
    free(vxsaved);
    free(vysaved);
}

void draw_springs(SDL_Renderer* renderer,point* points,spring* springs,int nb_springs){
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    for (int i = 0; i < nb_springs;i++){
        SDL_RenderDrawLine(renderer,points[springs[i].p1].pos.x,points[springs[i].p1].pos.y,points[springs[i].p2].pos.x,points[springs[i].p2].pos.y);
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]){
    if (argc != 3) return EXIT_FAILURE;
    int nb_points = atoi(argv[1]);
    int radius = atoi(argv[2]);
    vect2 center = {.x = SCREEN_WIDTH / 2, .y = SCREEN_HEIGHT / 2};
    point* points = build_sphere(nb_points,radius,center);
    spring* springs = create_springs(nb_points,points);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,SCREEN_WIDTH,SCREEN_HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;
    int running = 1;
    SDL_Event e;
    //PRESSURE = 1/(3.14 * radius * radius);
    
    while (running){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = 0;
                    break;
                
                default:
                    break;
                }
            }
        }
        printf("Current pressure : %f\n",PRESSURE);
        heun_integrator(springs,points,nb_points,radius);
        if (PRESSURE < FINAL_PRESSURE){
            PRESSURE += FINAL_PRESSURE / 5.0f;
        }
        draw_springs(renderer,points,springs,nb_points);
        SDL_Delay(16);
    }
    free(points);
    free(springs);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

// gcc soft_ball.c -o sb -Wall -Wvla -Wextra -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm