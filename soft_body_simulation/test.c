#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>


#define WIDTH 900
#define HEIGHT 900
#define SPACING 75
#define POINT_RADIUS 25
#define DT 0.016f
#define MATERIAL_FRICTION 0.8
#define BOUNCINESS 0.9


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


vect2 GRAVITY = {.x = 0.0f,.y = 981.0f};
//////////////////////////////////////////////////////////

struct Point;
struct Stick;

struct Point {
    vect2 pos;
    vect2 vel;
    vect2 force;
    vect2 prev_pos;
    float mass;
};

struct Stick {
    struct Point* p1;
    struct Point* p2;
    float stiffness;
    float length;
    float damping;
};

typedef struct Point point;
typedef struct Stick stick;


point** create_points(int startX,int startY,int rows,int columns){
    point** points = malloc(sizeof(point*) * rows);
    for (int i = 0; i < rows;i++){
        points[i] = malloc(sizeof(point) * columns);
        for (int j = 0; j < columns;j++){
            vect2 pos = {.x =  startX + j * SPACING,.y = startY + i * SPACING};
            vect2 force = {.x = 0, .y = 0};
            vect2 vel = {.x = 0,.y = 0};
            point p = {.pos = pos,.prev_pos = pos,.vel = vel,.force = force,.mass = 100.0f};
            points[i][j] = p;
        }
    }
    return points;
}

void delete_points(point** points,int rows){
    for (int i = 0; i < rows;i++){
        free(points[i]);
    }
    free(points);
}

void keep_inside_view(point* p){
    if (p->pos.x < 0.0) {
        vect2 dist = diff(p->pos,p->prev_pos);
        p->pos.x *= -1;
        p->prev_pos.x = p->pos.x + dist.y * BOUNCINESS;
        float j = dist.y;
        float k = dist.x * MATERIAL_FRICTION;
        float t = j;
        if (j != 0.0) t /= fabsf(j);
        if (fabsf(j) <= fabsf(k) && j * t > 0.0) p->pos.y -= 2.0f * j;
        else if (k * t > 0.0) p->pos.y -= k;
    } else if (p->pos.x > WIDTH) {
        vect2 dist = diff(p->pos,p->prev_pos);
        p->pos.x = 2.0f * WIDTH - p->pos.x;
        p->prev_pos.x = p->pos.x + BOUNCINESS * dist.y;
        float j = dist.y;
        float k = dist.x * MATERIAL_FRICTION;
        float t = j;
        if (j != 0.0) t /= fabsf(j);
        if (fabsf(j) <= fabsf(k) && j * t > 0.0) p->pos.y -= 2.0f * j;
        else if (k * t > 0.0) p->pos.y -= k;
    }

    if (p->pos.y < 0){
        vect2 dist = diff(p->pos,p->prev_pos);
        p->pos.y *= -1;
        p->prev_pos.y = p->pos.y + dist.x * BOUNCINESS;
        float j = dist.x;
        float k = dist.y * MATERIAL_FRICTION;
        float t = j;
        if (j != 0.0) t /= fabsf(j);
        if (fabsf(j) <= fabsf(k) && j * t > 0.0) p->pos.x -= 2.0f * j;
        else if (k * t > 0.0) p->pos.x -= k;

    } else if (p->pos.y > HEIGHT) {
        vect2 dist = diff(p->pos,p->prev_pos);
        p->pos.y = 2.0f * HEIGHT - p->pos.y;
        p->prev_pos.y = p->pos.y + BOUNCINESS * dist.x;
        float j = dist.x;
        float k = dist.y * MATERIAL_FRICTION;
        float t = j;
        if (j != 0.0) t /= fabsf(j);
        if (fabsf(j) <= fabsf(k) && j * t > 0.0) p->pos.x -= 2.0f * j;
        else if (k * t > 0.0) p->pos.x -= k;
    }
}

void update_point(point* p){
    p->vel =  diff(multiply(p->pos,2),p->prev_pos);
    p->prev_pos = p->pos;
    p->pos.x = p->vel.x +  DT * DT * p->force.x / p->mass;
    p->pos.y = p->vel.y +  DT * DT * p->force.y / p->mass;
    p->vel = diff(p->pos,p->prev_pos);
    p->force = zero_vect;
    keep_inside_view(p);
}

void apply_force(point* p,vect2 force){
    p->force.x += force.x;
    p->force.y += force.y;
}

///////

int get_nb_sticks(int rows,int columns){
    int count = 0;
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (i != 0 && j != columns - 1) count++;
            if (i != 0 && j != 0) count++;
            if (i != 0) count++;
            if (j != 0) count++;
        }
    }
    return count;
}

stick* create_new_stick(point* p1,point* p2){
    stick* s = malloc(sizeof(stick));
    s->p1 = p1;
    s->p2 = p2;
    s->length = vector_magnitude(diff(p1->pos,p2->pos));
    s->stiffness = 0.9f;
    s->damping = 0.9f;
    return s;
}

stick** create_sticks(point** points,int rows,int columns,int* nb_sticks){
    *nb_sticks = get_nb_sticks(rows,columns);
    stick** sticks = malloc(sizeof(stick*) * *nb_sticks);
    int index = 0;
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (i != 0 && j != columns - 1){
                stick* s = create_new_stick(&points[i][j],&points[i - 1][j + 1]); //top right point
                sticks[index] = s;
                index++;
            }
            if (i != 0 && j != 0){
                stick* s = create_new_stick(&points[i][j],&points[i - 1][j - 1]); //top-left point
                sticks[index] = s;
                index++;
            }
            if (j != 0){
                stick* s = create_new_stick(&points[i][j],&points[i][j - 1]); //left point
                sticks[index] = s;
                index++;
            }
            if (i != 0){
                stick* s = create_new_stick(&points[i][j],&points[i - 1][j]); //Top point
                sticks[index] = s;
                index++;
            }
        }
    }
    return sticks;
}


void update_stick(stick* s){
    vect2 difference = diff(s->p2->pos,s->p1->pos);
    float difference_length = vector_magnitude(difference);
    vect2 difference_normalized = normalize(difference);
    vect2 F;
    F.x = 0.5f * s->stiffness * (difference_length - s->length) * difference_normalized.x;
    F.y = 0.5f * s->stiffness * (difference_length - s->length) * difference_normalized.y;
    apply_force(s->p1,F);
    apply_force(s->p2,multiply(F,-1.0f));
}

void update(point** points,int rows, int columns,stick** sticks,int nb_sticks){
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            apply_force(&points[i][j],multiply(GRAVITY,points[i][j].mass));
            update_point(&points[i][j]);
        }
    }
    for (int i = 0; i < nb_sticks;i++){
        update_stick(sticks[i]);
    }
}

void draw_stick(SDL_Renderer* renderer,stick* s){
    SDL_RenderDrawLine(renderer,s->p1->pos.x,s->p1->pos.y,s->p2->pos.x,s->p2->pos.y);
}

void render(SDL_Renderer* renderer, stick** sticks, int nb_sticks){
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    for (int i = 0; i < nb_sticks;i++){
        draw_stick(renderer,sticks[i]);
    }
    SDL_RenderPresent(renderer);
}

int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}


int main(int argc, char* argv[]){
    if (argc != 3) return EXIT_FAILURE;
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,1200,700,"test");
    if (status == 1) return EXIT_FAILURE;

    point** points = create_points(100,100,rows,columns);
    int nb_sticks;
    stick** sticks = create_sticks(points,rows,columns,&nb_sticks);
    int running = 1;
    SDL_Event e;
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
        update(points,rows,columns,sticks,nb_sticks);
        render(renderer,sticks,nb_sticks);
        SDL_Delay(16);
    }
    delete_points(points,rows);
    for (int i = 0; i < nb_sticks;i++){
        free(sticks[i]);
    }
    free(sticks);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}


//gcc test.c -o test -Wall -Wvla -Wextra -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm
