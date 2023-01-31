#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>  

#define WIDTH 1500
#define HEIGHT 900
#define FRAME_DELAY 16

#define GRAVITY 1
#define WALL_BOUNCINESS 0.5
#define BALL_BOUNCINESS 0.9
#define GROUND_FRICTION 0.9
#define GROUND_BOUNCINESS 0.9
#define NB_BALLS 2

#define BALL_RADIUS 30

int is_distance(int x_pos_0,int y_pos_0,int x_pos_1,int y_pos_1,float distance){
    int horiz_dist = x_pos_1 - x_pos_0;
    int vert_dist = y_pos_1 - y_pos_0;
    float points_dist = (
        pow((float)horiz_dist, 2.0)
        + pow((float)vert_dist, 2.0)
    );

    return (points_dist <= sqrt(distance));
}

struct vect2 {
    float x;
    float y;
};

typedef struct vect2 vect2;

struct Ball {
    float x;
    float y;
    float velX;
    float velY;
    int radius;
    int mass;
};

typedef struct Ball ball;

ball* create_ball(float x,float y,float velX,float velY,int radius,int mass){
    ball* b = malloc(sizeof(ball));
    b->x = x;
    b->y = y;
    b->velX = velX;
    b->velY = velY;
    b->radius = radius;
    b->mass = mass;
    return b;
}

void copy_ball(ball* source,ball* dest){
    dest->x = source->x;
    dest->y = source->y;
    dest->velX = source->velX;
    dest->velY = source->velY;
    dest->radius = source->radius;
    dest->mass = source->mass;
}

float rand_between(int l, int r) {
  return ( (rand() / (RAND_MAX * 1.0f)) * (r - l) + l);
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


int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}

void update_ball(ball* b){
    b->x += b->velX;
    b->y += b->velY;
    b->velY += GRAVITY;
}


bool check_ground_collision(ball* b){
    if (b->y + b->radius > HEIGHT){
        b->y = HEIGHT - b->radius;
        b->velY *= GROUND_BOUNCINESS;
        return true;
    } else if (b->y - b->radius < 0){
        b->y = b->radius;
        b->velY *= -GROUND_BOUNCINESS;
    }
    return false;
}

bool check_wall_collision(ball* b){
    if (b->x + b->radius > WIDTH){
        b->x = WIDTH - b->radius;
        return true;
    } else if (b->x - b->radius < 0){
        b->x = b->radius;
        return true;
    }
    return false;
}

float abs_float(float n){
    return n > 0 ? n:-n;
}


bool check_ball_x_collision(ball* b1,ball* b2){
    return abs_float(b1->x - b2->x) < 5 + b1->radius + b2->radius;
}

bool check_ball_y_collision(ball* b1,ball* b2){
    return abs_float(b1->y - b2->y) < 5 + b1->radius + b2->radius;
}


float velocity_magnitude(ball* b1){
    return sqrt(b1->velX * b1->velX + b1->velY * b1->velY);
}

float vector_magnitude(vect2 v){
    return sqrt(v.x * v.x + v.y * v.y);
}

float dot_product(vect2 v1,vect2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}

void do_elastic_collision(ball* b1,ball* b2){
    //Norme quasi égale à 0
    float magnitude_vel_b1 = velocity_magnitude(b1);
    float magnitude_vel_b2 = velocity_magnitude(b2);
    if (magnitude_vel_b1 < 1e-5 && magnitude_vel_b2 < 1e-5) return;

    //On récup le vecteur normal à la surface de collision
    vect2 v_n = {.x= b2->x - b1->x,.y = b2->y - b1->y};
    float v_n_magnitude = vector_magnitude(v_n);
    vect2 v_un = {.x = v_n.x / v_n_magnitude,.y = v_n.y/ v_n_magnitude}; //On le normalise

    
    vect2 v_ut = {.x= -v_un.y,.y=v_un.x};

    vect2 vel1 = {.x=b1->velX,.y = b1->velY};
    vect2 vel2 = {.x=b2->velX,.y = b2->velY};

    double v1n = dot_product(v_un,vel1);
    double v1t = dot_product(v_ut,vel1);
    double v2n = dot_product(v_un,vel2);
    double v2t = dot_product(v_ut,vel2);

    double v1n_f = (v1n * (b1->mass - b2->mass) + 2. * b2->mass * v2n) / (b1->mass + b2->mass);
    double v2n_f = (v2n * (b2->mass - b1->mass) + 2. * b1->mass * v1n) / (b1->mass + b2->mass);

    vect2 v_v1nprime = {.x= v1n_f * v_un.x,.y = v1n_f * v_un.y};
    vect2 v_v1tPrime = {.x= v1t * v_ut.x,.y =v1t * v_ut.y};
	vect2  v_v2nPrime = {.x= v2n_f * v_un.x,.y = v2n_f * v_un.y};
	vect2  v_v2tPrime = {.x= v2t * v_ut.x,.y =v2t * v_ut.y};

    b1->velX = v_v1nprime.x + v_v1tPrime.x;
    b1->velY = v_v1nprime.y + v_v1tPrime.y;
    b2->velX = v_v2nPrime.x + v_v2tPrime.x;
    b2->velY = v_v2nPrime.y + v_v2tPrime.y;
}


int main(void){
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;
    int running = 1;
    SDL_Event e;
    ball** tab_balls = malloc(sizeof(ball*) * NB_BALLS);
    ball** prev_tab_balls = malloc(sizeof(ball*) * NB_BALLS);
    for (int i = 0; i < NB_BALLS;i++){
        tab_balls[i] = create_ball((i + 1) * WIDTH/4,HEIGHT/2,10,-10,BALL_RADIUS,20);
        prev_tab_balls[i] = create_ball((i + 1) * WIDTH/4,HEIGHT/2,10,-10,BALL_RADIUS,20);
    }

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    for (int i = 0; i < NB_BALLS;i++){
        DrawCircle(renderer,tab_balls[i]->x,tab_balls[i]->y,BALL_RADIUS);
    }
    SDL_RenderPresent(renderer);

    while (running){
        SDL_Delay(FRAME_DELAY);

        for (int i = 0; i < NB_BALLS;i++){
            copy_ball(tab_balls[i],prev_tab_balls[i]);
            update_ball(tab_balls[i]);
        }

        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);


        for (int i = 0; i < NB_BALLS;i++){
            if (check_ground_collision(tab_balls[i])){
                tab_balls[i]->velY *= -0.8;
                if (abs_float(tab_balls[i]->velY) < 2.5){
                    tab_balls[i]->velX *= GROUND_FRICTION;
                    tab_balls[i]->velY = 0;
                } 
            }
        }

        for (int i = 0; i < NB_BALLS;i++){
            if (check_wall_collision(tab_balls[i])) {
                tab_balls[i]->velX *= -WALL_BOUNCINESS;
            }
        }

        for (int i = 0; i < NB_BALLS;i++){
            for (int j = i; j < NB_BALLS;j++){
                if (check_ball_y_collision(tab_balls[i],tab_balls[j]) && check_ball_x_collision(tab_balls[i],tab_balls[j])){
                    do_elastic_collision(tab_balls[i],tab_balls[j]);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        for (int i = 0; i < NB_BALLS;i++){
            DrawCircle(renderer,tab_balls[i]->x,tab_balls[i]->y,BALL_RADIUS);
        }
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                case SDLK_q:
                    running = 0;
                    break;
                default:
                    break;
                }
            }
        }
    }
    for (int i = 0; i < NB_BALLS;i++){
        free(tab_balls[i]);
        free(prev_tab_balls[i]);
    }
    free(tab_balls);
    free(prev_tab_balls);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

//gcc user_input_velocity_test.c -o uivt -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs) -lSDL2 -lm
