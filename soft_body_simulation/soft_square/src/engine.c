#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../include/point.h"
#include "../include/spring.h"


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define DT 0.016


bool MOVED = false;


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


void draw_spring(SDL_Renderer* renderer,spring* s){
      int p1_x = s->p1->pos.x;
      int p1_y = s->p1->pos.y;
      int p2_x = s->p2->pos.x;
      int p2_y = s->p2->pos.y;
      SDL_RenderDrawLine(renderer,p1_x,p1_y,p2_x,p2_y);
}


void verlet_step(SDL_Renderer* renderer,point** points,spring** springs,double dt,int nb_springs,int rows,int columns){

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    
    clear_forces(points,rows,columns);
    for (int i = 0; i < nb_springs;i++){
        update_spring(springs[i]);
        draw_spring(renderer,springs[i]);
    }

    update_positions(points,dt,rows,columns);
    update_velocities(points,dt,rows,columns);

    for (int i = 0; i < rows;i++){
      for (int j = 0; j < columns;j++){
        int x = points[i][j].pos.x;
        int y = points[i][j].pos.y;
        DrawCircle(renderer,x,y,10);
      }
    }
    SDL_RenderPresent(renderer);
}

void change_vel(point* p,double dy){
  p->prev_pos.y = p->pos.y;
  p->pos.y += dy;
}

int events_handling(point** points,int rows,int columns){
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        switch (e.type) {
          case SDL_QUIT:
              return 1;
          case SDL_KEYDOWN: {
              if (e.key.keysym.sym == SDLK_ESCAPE) return 1;
              if (e.key.keysym.sym == SDLK_LEFT) {
                for (int i = 0; i < rows;i++){
                  for (int j = 0; j < columns;j++){
                    vect2 f = {.x = -500.0,.y = 0.0};
                    apply_force(&points[i][j],f);
                  }         
                }
              }
              if (e.key.keysym.sym == SDLK_RIGHT) {
                for (int i = 0; i < rows;i++){
                  for (int j = 0; j < columns;j++){
                    vect2 f = {.x = 500.0,.y = 0.0};
                    apply_force(&points[i][j],f);
                  }         
                }
              }
              if (e.key.keysym.sym == SDLK_SPACE) {
                printf("ok");
                  for (int i = 0;i < rows;i++){
                    for (int j = 0; j < columns;j++){
                      change_vel(&points[i][j],-2.0);
                    }
                }
              }
            }
          default:
              break;
        } 
    }
  return 0;
}


int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}


int main(int argc,char* argv[]){
    if (argc != 3) return EXIT_FAILURE;
    int rows = atoi(argv[1]);
    int columns = atoi(argv[1]);
    point** points = create_points(100,100,rows,columns,75);
    int nb_springs;
    spring** springs = create_springs(points,rows,columns,&nb_springs);


    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,SCREEN_WIDTH,SCREEN_HEIGHT,"Tuned Mass Damper simulation, by Esteban795.");
    if (status == 1) return EXIT_FAILURE;
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);

    while (true) {
        int res = events_handling(points,rows,columns);
        if (res == 1) break; // exits program   
        verlet_step(renderer,points,springs,DT,nb_springs,rows,columns);
        SDL_Delay(6);
    }

    delete_points(points,rows);
    delete_springs(springs,nb_springs);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
