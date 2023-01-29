#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800

#define BALL_BOUCINESS 0.6
#define WALL_BOUCINESS 0.8

#define GRAVITY 0.5

#define MOMENTUM_LOST 0.002
#define NB_BALLS 3
#define FRAME_DELAY 6


struct Ball {
    float x;
    float y;
    float velX;
    float velY;
};

typedef struct Ball ball;

int is_distance(int x_pos_0,int y_pos_0,int x_pos_1,int y_pos_1,float distance){
    int horiz_dist = x_pos_1 - x_pos_0;
    int vert_dist = y_pos_1 - y_pos_0;
    float points_dist = (
        pow((float)horiz_dist, 2.0)
        + pow((float)vert_dist, 2.0)
    );

    return (points_dist <= sqrt(distance));
}


ball* create_ball(float x,float y,float velX,float velY){
    ball* b = malloc(sizeof(ball));
    b->x = x;
    b->y = y;
    b->velX = velX;
    b->velY = velY;
    return b;
}

void copy_ball(ball* source,ball* dest){
    dest->x = source->x;
    dest->y = source->y;
    dest->velX = source->velX;
    dest->velY = source->velY;
}

void update_ball(ball* b,float Xgrav, float Ygrav){
    b->x += b->velX;
    b->y += b->velY;
    b->velX *= (1 - MOMENTUM_LOST);
    b->velY *= (1 - MOMENTUM_LOST);
    if (b->x < 0 || b->x > WIDTH) {
        printf("dépassé");
        b->velX *= -1;
    }
    if (b->y < 0 || b->y > HEIGHT){
        printf("dépassé");
        b->velY *= -1;
    }
    //Action of gravity source
    float delta_x = b->x - Xgrav;
    float delta_y = b->y - Ygrav;
    float hypothenuse = sqrt(delta_x * delta_x + delta_y * delta_y);
    b->velX = GRAVITY * delta_x/hypothenuse;
    b->velY = GRAVITY * delta_y/hypothenuse;
}

void collision_handling(ball* b1, ball* b2,int bounce_direction){
    b1->velX = b2->velY;
    b1->velY = b2->velX;
    switch (bounce_direction) {
        case 0:
            b1->x -= 1;
            break;
        case 1:
            b1->y += 1;
            break;
        case 2:
            b1->x += 1;
            break;
        case 3:
            b1->y -= 1;
            break;
        default:
            break;
    }
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




float rand_between(int l, int r) {
  return ( (rand() / (RAND_MAX * 1.0f)) * (r - l) + l);
}



int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}


int main(void){
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;

    int running = 1;
    SDL_Event e;
    int mouse_x,mouse_y,prev_mouse_x,prev_mouse_y;
    ball* mouse_ball = create_ball(0,0,0,0);

    ball** balls = malloc(NB_BALLS * sizeof(ball*));

    for (int i = 0; i < NB_BALLS;i++){
        //float x = rand_between(10,WIDTH - 10);
        //float y = rand_between(10,HEIGHT - 10);
        balls[i] = create_ball(WIDTH/2,HEIGHT/2,0,0);
    }

    ball** prev_frame_balls = malloc(NB_BALLS * sizeof(ball*));

    for (int i = 0; i < NB_BALLS; i++) {
        prev_frame_balls[i] = create_ball(0, 0, 0, 0);
    }

    while (running){
        SDL_Delay(FRAME_DELAY);
        prev_mouse_x = mouse_x;
        prev_mouse_y = mouse_y;

        SDL_GetMouseState(&mouse_x,&mouse_y);
        mouse_ball->x = (float) mouse_x;
        mouse_ball->y = (float) mouse_y;
        mouse_ball->velX = (float) mouse_x - prev_mouse_x;
        mouse_ball->velY = (float) mouse_y - prev_mouse_y;

        for (int i = 0; i < NB_BALLS;i++){
            copy_ball(balls[i],prev_frame_balls[i]);
        }
        int bounce_direction = 0;
        for (int i = 0; i < NB_BALLS;i++){
            bounce_direction %= 3;
            for (int j = 0; j < NB_BALLS;j++){
                bounce_direction %= 3;
                if (i == j) continue;
                if (is_distance((int)balls[i]->x,(int)balls[i]->y,(int)prev_frame_balls[j]->x,(int)prev_frame_balls[j]->y,1.5)){
                    collision_handling(balls[i],prev_frame_balls[j],bounce_direction);
                    bounce_direction++;
                }
            }
            if (is_distance((int)balls[i]->x,(int)balls[i]->y,mouse_x,mouse_y,1.5)){
                collision_handling(balls[i],mouse_ball,bounce_direction);
                bounce_direction++;
            }
            update_ball(balls[i],mouse_x,mouse_y);
        }
        SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
        for (int i = 0; i < NB_BALLS;i++){
            DrawCircle(renderer,balls[i]->x,balls[i]->y,20);
        }
        SDL_RenderPresent(renderer);

        
        // mouse particle
        SDL_SetRenderDrawColor(renderer, 255, 10, 0, 255);
        // and a little bigger
        DrawCircle(renderer,mouse_x,mouse_y,30);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym)
                {
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
        free(balls[i]);
    }
    free(balls);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free(mouse_ball);
    return 0;
}

//gcc main.c -o main -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs) -lSDL2 