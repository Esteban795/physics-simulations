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


bool GRAVITY_ON = true;


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
    if (GRAVITY_ON) b->velY += GRAVITY;
}


bool checkCollision(ball* b){
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


void apply_collision(ball* b1,ball* b2){
    float b1_velX = b1->velX;
    float b1_velY = b1->velY;
    b1->velX = (b1->velX * (b1->mass - b2->mass) + 2 * b2->mass * b2->velX)/(b1->mass + b2->mass);
    b1->velY = (b1->velY * (b1->mass - b2->mass) + 2 * b2->mass * b2->velY)/(b1->mass + b2->mass);
    b2->velX = (b2->velX * (b2->mass - b1->mass) + 2 * b1->mass * b1_velX)/(b1->mass + b2->mass);
    b2->velY = (b2->velY * (b2->mass - b1->mass) + 2 * b1->mass * b1_velY)/(b1->mass + b2->mass);
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
    ball* b1 = create_ball(WIDTH/4,HEIGHT/2 - 100,10,0,30,20);
    ball* b2 = create_ball(2 * WIDTH/4,HEIGHT/2,-10,0,30,10);
    ball* b3 = create_ball(3 * WIDTH/4,HEIGHT/2,10,20,30,10);


    /*ball* prev_b1 = create_ball(WIDTH/4,HEIGHT/2 - 100,3,0,30,20);
    ball* prev_b2 = create_ball(2 * WIDTH/4,HEIGHT/2,-3,0,30,10);
    ball* prev_b3 = create_ball(3 * WIDTH/4,HEIGHT/2,3,20,30,10);*/
    int bup_mouse_x,bup_mouse_y;
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    DrawCircle(renderer,b1->x,b1->y,b1->radius);
    SDL_SetRenderDrawColor(renderer,0,0,255,255);
    DrawCircle(renderer,b2->x,b2->y,b2->radius);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    DrawCircle(renderer,b3->x,b3->y,b3->radius);
    SDL_RenderPresent(renderer);

    while (running){
        SDL_Delay(FRAME_DELAY);

        /*copy_ball(b1,prev_b1);
        copy_ball(b2,prev_b2);
        copy_ball(b3,prev_b3);*/
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);

        update_ball(b1);
        update_ball(b2);
        update_ball(b3);
        if (checkCollision(b1)){
            b1->velY *= -0.8;
            if (abs_float(b1->velY) < 4){
                b1->velX *= GROUND_FRICTION;
                b1->velY = 0;
            } 
        }
        if (checkCollision(b2)){
            b2->velY *= -0.8;
            if (abs_float(b2->velY) < 4){
                b2->velX *= GROUND_FRICTION;
                b2->velY = 0;
            } 
        }
        if (checkCollision(b3)){
            b3->velY *= -0.8;
            if (abs_float(b3->velY) < 4){
                b3->velX *= GROUND_FRICTION;
                b3->velY = 0;
            } 
        }
        if (check_wall_collision(b1)) b1->velX *= -WALL_BOUNCINESS;
        if (check_wall_collision(b2)) b2->velX *= -WALL_BOUNCINESS;
        if (check_wall_collision(b3)) b3->velX *= -WALL_BOUNCINESS;

        if (check_ball_y_collision(b1,b2) && check_ball_x_collision(b1,b2)){
            //apply_collision(b1,b2);
            do_elastic_collision(b1,b2);
        }
        if (check_ball_y_collision(b1,b3) && check_ball_x_collision(b1,b3)){
            //apply_collision(b1,b3);
            do_elastic_collision(b1,b3);
        }
        if (check_ball_y_collision(b2,b3) && check_ball_x_collision(b2,b3)){
            //apply_collision(b2,b3);
            do_elastic_collision(b2,b3);
        }
        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        DrawCircle(renderer,(int)b1->x,(int)b1->y,b1->radius);
        SDL_SetRenderDrawColor(renderer,0,0,255,255);
        DrawCircle(renderer,(int)b2->x,(int)b2->y,b2->radius);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        DrawCircle(renderer,(int)b3->x,(int)b3->y,b3->radius);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                case SDLK_q:
                    running = 0;
                    break;
                case SDLK_g:
                    GRAVITY_ON = !GRAVITY_ON;
                default:
                    break;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP){
                SDL_GetMouseState(&bup_mouse_x,&bup_mouse_y);
                b1->velY = (bup_mouse_y - b1->y)/30;
                b1->velX = (bup_mouse_x - b1->x)/30;
            }
        }
    }
    free(b1);
    free(b2);
    free(b3);
    /*free(prev_b1);
    free(prev_b2);
    free(prev_b3);*/
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

//gcc user_input_velocity.c -o uiv -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs) -lSDL2 -lm
