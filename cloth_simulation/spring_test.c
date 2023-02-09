#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>


#define WIDTH 700
#define HEIGHT 700
#define PARTICLE_RADIUS 15
#define FRAME_DELAY 16
#define dt 0.5


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





void keep_inside_view(particle* p){
    if (p->y + PARTICLE_RADIUS >= HEIGHT) p->y = HEIGHT - PARTICLE_RADIUS - 10;
}



void draw(SDL_Renderer* renderer,particle* particles,stick* sticks,int n,int stick_len){
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    for (int i = 0; i < n;i++){
        DrawCircle(renderer,particles[i].x,particles[i].y,PARTICLE_RADIUS);
    }
    for (int i = 0; i < stick_len;i++){
        SDL_RenderDrawLine(renderer,sticks[i].p1->x,sticks[i].p1->y,sticks[i].p2->x,sticks[i].p2->y);
    }
    SDL_RenderPresent(renderer);
}


particle* predefinies(){
    particle* particles = malloc(sizeof(particle) * 4);
    particles[0].x = 220;
    particles[0].y = 120;
    particles[0].mass = 10;
    particles[0].prevx = particles[0].x;
    particles[0].prevy = particles[0].y;
    particles[1].x = 420;
    particles[1].y = 120;
    particles[1].mass = 10;
    particles[1].prevx = particles[1].x;
    particles[1].prevy = particles[1].y;
    particles[2].x = 420;
    particles[2].y = 320;
    particles[2].mass = 10;
    particles[2].prevx = particles[2].x;
    particles[2].prevy = particles[2].y;
    particles[3].x = 220;
    particles[3].y = 320;
    particles[3].mass = 10;
    particles[3].prevx = particles[3].x;
    particles[3].prevy = particles[3].y;
    return particles;
}

void print_stick(stick s){
    print_particle(*s.p1);
    printf("\n");
    print_particle(*s.p2);
    printf("\n");
    printf("Length : %.2f",s.length);
    printf("\n");
    printf("\n");
    printf("\n");
}
int main(int argc,char* argv[]){
    if (argc != 2) return EXIT_FAILURE;
    int nb_particles = atoi(argv[1]);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;
    particle* particles = predefinies();
    stick* sticks = create_sticks(particles,nb_particles);
    int stick_len = nb_particles + 2;
    int running = 1;
    SDL_Event e;
    while (running){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                case SDLK_q:
                    running = 0;
                    break;
                }
            }
        }
        update(particles,sticks,nb_particles,stick_len);
        draw(renderer,particles,sticks,nb_particles,stick_len);
        SDL_Delay(FRAME_DELAY);
    }
    free(particles);
    free(sticks);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

//gcc spring_test.c -o st -W -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm
