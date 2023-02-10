
#include <SDL2/SDL.h>
#include <stdbool.h>


#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700
#define SPACING 100


struct vect2 {
    float x;
    float y;
};

typedef struct vect2 vect2;

vect2 diff(vect2 v1,vect2 v2){
    vect2 temp = {.x = v1.x - v2.x,.y = v1.y - v2.y};
    return temp;
}

vect2 divide(vect2 v,float n){
    vect2 temp = {.x = v.x / n,.y = v.y / n};
    return temp;
}

float vector_magnitude(vect2 v){
    return sqrt(v.x * v.x + v.y * v.y);
}

float dot_product(vect2 v1,vect2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}

float get_length(vect2 p){
    return sqrt(p.x * p.x + p.y * p.y);
}

struct Stick;
struct Particle;

struct Stick {
    struct Particle* p1;
    struct Particle* p2;
    float length;
    bool is_active;
    bool is_selected;
    bool is_null;
};



struct Particle {
    struct Stick* sticks;
    float x;
    float y;
    float prevx;
    float prevy;
    float initx;
    float inity;
    float mass;
    bool is_pinned;
    bool is_selected;
};

typedef struct Stick stick;
typedef struct Particle particle;

float dist(particle p1,particle p2){
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
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

void add_stick(particle* p,stick s,int index){
    p->sticks[index] = s;
}

void print_stick(stick s){
    printf("P1 (%f,%f),P2 : (%f,%f), length : %f\n",s.p1->x,s.p1->y,s.p2->x,s.p2->y,s.length);
}

int get_nb_sticks(int rows,int columns){
    int count = 0;
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (i != 0) count += 1;
            if (j != 0) count += 1;
        }
    }
    return count;
}

stick create_new_stick(particle* p1,particle* p2){
    stick s = {.p1 = p1,.p2 = p2,.length = dist(*p1,*p2),.is_active = true,.is_selected = false,.is_null = false};
    return s;
}

stick* create_sticks(particle** particles,int rows,int columns,int* nb_sticks){
    *nb_sticks = get_nb_sticks(rows,columns);
    stick* sticks = malloc(sizeof(stick) * *nb_sticks);
    int index = 0;
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (j != 0){
                stick s = create_new_stick(&particles[i][j],&particles[i][j - 1]); //left particle
                add_stick(&particles[i][j - 1],s,0);
                add_stick(&particles[i][j],s,0);
                sticks[index] = s;
                index++;
            }
            if (i != 0){
                stick s = create_new_stick(&particles[i][j],&particles[i - 1][j]); //Top particle
                add_stick(&particles[i - 1][j],s,1);
                add_stick(&particles[i][j],s,1);
                sticks[index] = s;
                index++;
            }
            if (i == 0 && j % 2 == 0){
                particles[i][j].is_pinned = true;
            }
        }
    }
    return sticks;
}

void update_stick(stick* s){
    if (!s->is_active) return;

    vect2 p1_pos = {.x = s->p1->x , .y = s->p1->y};
    vect2 p2_pos = {.x = s->p2->x, .y = s->p2->y};

    vect2 difference = diff(p1_pos,p2_pos);
    
    float distance = sqrtf(difference.x * difference.x + difference.y * difference.y);
    float diff_factor = (s->length - distance) / distance;

    vect2 offset = {.x = difference.x * diff_factor * 0.5f, .y = difference.y * diff_factor * 0.5f};

    if (!s->p1->is_pinned){
        s->p1->x = p1_pos.x + offset.x;
        s->p1->y = p1_pos.y + offset.y;
    }
    if (!s->p2->is_pinned){
        s->p2->x = p2_pos.x - offset.x;
        s->p2->y = p2_pos.y - offset.y;
    }
}

void draw_stick(SDL_Renderer* renderer,stick s){
    if (!s.is_active) return;
    SDL_RenderDrawLine(renderer,s.p1->x,s.p1->y,s.p2->x,s.p2->y);
}


/*
Get euclidean distance between two particles.
*/

/*
Returns a vect2 with difference of x and y
*/
vect2 get_diff(particle* p1,particle* p2){
    vect2 temp = {.x = p1->x - p2->x,.y = p1->y - p2->y};
    return temp;
}

void print_particle(particle p){
    printf("position : (%.3f,%.3f), prev position : (%.3f,%.3f), pinned : %d\n",p.x,p.y,p.prevx,p.prevy,p.is_pinned);
}



particle** create_particles(int startX,int startY,int width,int height,int spacing){
    particle** particles = malloc(sizeof(particle*) * height);
    for (int i = 0; i < height;i++){
        particles[i] = malloc(sizeof(particle) * width);
        for (int j = 0; j < width;j++){
            particle p = {.mass = 10,.x = startX + j * spacing,.y = startY + i * spacing};
            p.prevx = p.initx = p.x;
            p.prevy = p.inity = p.y;
            p.is_pinned = false;
            p.is_selected = false;
            p.sticks = malloc(sizeof(stick) * 2);
            p.sticks[0].is_null = true;
            p.sticks[1].is_null = true;
            particles[i][j] = p;
        }
    }
    return particles;
}

void keep_inside_view(particle* p,int width,int height){
    if (p->x > width){
        p->x = width;
        p->prevx = p->x;
    }
    if (p->x < 0){
        p->x = 0;
        p->prevx = p->x;
    }
    if (p->y > height){
        p->y = height;
        p->prevy = p->y;
    }
    if (p->y < 0){
        p->y = 0;
        p->prevy = p->y;
    }
}


void update_particle(particle* p,float dt,float drag, vect2 acceleration,float elasticity){
    /*
    vect2 mouse_pos = m->pos;
    vect2 cursorToPosDir = {.x = p->x - mouse_pos.x,.y = p->y - mouse_pos.y};
    float cursorToPosDist = cursorToPosDir.x * cursorToPosDir.x + cursorToPosDir.y * cursorToPosDir.y;
    p->is_selected = cursorToPosDist < m->cursor_size * m->cursor_size;
    
    if (!p->sticks[0].is_null) p->sticks[0].is_selected = p->is_selected;
    if (!p->sticks[1].is_null) p->sticks[1].is_selected = p->is_selected;
    */
    if (p->is_pinned){
        p->x = p->initx;
        p->y = p->inity;
        return;
    }
    float new_x = p->x + (p->x - p->prevx) * (1.0f - drag) + acceleration.x * (1.0f - drag) * dt * dt;
    float new_y = p->y + (p->y - p->prevy) * (1.0f - drag) + acceleration.y * (1.0f - drag) * dt * dt;
    p->prevx = p->x;
    p->prevy = p->y;
    p->x = new_x;
    p->y = new_y;
    //keep_inside_view(p,SCREEN_WIDTH,SCREEN_HEIGHT);
}

int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}

void update(particle** particles,int rows,int columns,stick* sticks,int nb_sticks){
    vect2 grav = {.x = 0.0f,.y = 981.0f};
    for (int i = 0;i < rows;i++){
        for (int j = 0; j < columns;j++){
            update_particle(&particles[i][j],0.01f,0.01f,grav,10.0f);
        }
    }
    for (int i = 0; i < nb_sticks;i++){
        update_stick(&sticks[i]);
    }
}


void render(SDL_Renderer* renderer,particle** particles,int rows,int columns,stick* sticks,int nb_sticks){
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            DrawCircle(renderer,particles[i][j].x,particles[i][j].y,20);
        }
    }
    for (int i = 0; i < nb_sticks;i++){
        draw_stick(renderer,sticks[i]);
    }
    SDL_RenderPresent(renderer);
}

void free_all(particle** particles,int rows,int columns,stick* sticks){
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            free(particles[i][j].sticks);
        }
        free(particles[i]);
    }
    free(particles);
    free(sticks);
}

int main(int argc, char* argv[]){
    if (argc != 3) return EXIT_FAILURE;
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    particle** particles = create_particles(100,100,columns,rows,SPACING);
    int nb_sticks;
    stick* sticks = create_sticks(particles,rows,columns,&nb_sticks);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,SCREEN_WIDTH,SCREEN_HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;
    SDL_Event e;
    int running = 1;
    int count = 0;
    for (int i = 0; i < rows;i++){
        for (int j = 0;j < columns;j++){
            print_particle(particles[i][j]);
        }
    }
    while (running && count < 100){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                    case SDLK_q:
                        running = 0;
                        break;
                }
            }
        }
        update(particles,rows,columns,sticks,nb_sticks);
        render(renderer,particles,rows,columns,sticks,nb_sticks);
        SDL_Delay(100);
        count++;
    }
    free_all(particles,rows,columns,sticks);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

}

//gcc particle.c -o particle -Wall -Wvla -Wextra -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm