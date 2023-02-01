#include "../include/engine.h"



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


bool check_x_bound(ball b){
  return (b.x - b.radius) > 0 && (b.x + b.radius < WIDTH);
}

bool check_y_bound(ball b){
  return (b.y - b.radius > 0) && (b.y + b.radius < HEIGHT);
}

bool check_ground_collision(ball* b){
    if (b->y + b->radius > HEIGHT){
        b->y = HEIGHT - b->radius;
        return true;
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


void update_ball(ball* b){
    b->x += b->velX;
    b->y += b->velY;
    b->velY += GRAVITY;
}

void main_loop(SDL_Renderer* renderer){
    int running = 1;
    SDL_Event e;
    float init_velX = rand_between(0,50);
    float init_velY =  rand_between(0,50);
    ball* current_frame_ball = create_ball(WIDTH/2,HEIGHT/2,init_velX,init_velY,30);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    DrawCircle(renderer,current_frame_ball->x,current_frame_ball->y,current_frame_ball->radius);
    SDL_RenderPresent(renderer);


    while (running){
        SDL_Delay(FRAME_DELAY);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);

        update_ball(current_frame_ball);
        if (check_ground_collision(current_frame_ball)){
            current_frame_ball->velY = -1 * GROUND_BOUNCINESS * current_frame_ball->velY;
            if (abs_float(current_frame_ball->velY) < 2){
                current_frame_ball->velX *= GROUND_FRICTION;
                current_frame_ball->velY = 0;
            } 
        }
        if (check_wall_collision(current_frame_ball)){
            current_frame_ball->velX = -1 * WALL_BOUNCINESS * current_frame_ball->velX;
        }
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        DrawCircle(renderer,(int)current_frame_ball->x,(int)current_frame_ball->y,current_frame_ball->radius);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                case SDLK_q:
                    running = 0;
                    break;
                case SDLK_1:
                    current_frame_ball->velX = rand_between(10,50) - 30;
                    current_frame_ball->velY = -rand_between(20,40);
                default:
                    break;
                }
            }
        }
    }
    free(current_frame_ball);
}