#include <SDL2/SDL.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700
#define SPACING 10


//random ass function

int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}



// VECT2

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
    return sqrtf(v.x * v.x + v.y * v.y);
}

float dot_product(vect2 v1,vect2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}

float get_length(vect2 p){
    return sqrt(p.x * p.x + p.y * p.y);
}

///////////////////////////////////////////////////////////////////////

// MOUSE
struct Mouse {
    vect2 pos;
    vect2 prev_pos;
    bool left_button_down;
    bool right_button_down;
    float cursor_size;
    float max_cursor_size;
    float min_cursor_size;
};

typedef struct Mouse mouse;


mouse* mouse_create(float x,float y){
    mouse* m = malloc(sizeof(mouse));
    vect2 pos = {.x = x,.y = y};
    m->pos = pos;
    m->prev_pos = pos;
    m->cursor_size = 20;
    m->left_button_down = false;
    m->right_button_down = false;
    m->max_cursor_size = 100;
    m->min_cursor_size = 20;
    return m;
}

vect2 mouse_get_pos(mouse* m){
    return m->pos;
}

vect2 mouse_get_prevpos(mouse* m){
    return m->prev_pos;
}

vect2 mouse_diff_pos(mouse* m){
    vect2 temp = {.x = m->pos.x - m->prev_pos.x,.y = m->pos.y - m->prev_pos.y};
    return temp;
}

void update_pos(mouse* m,int x,int y){
    m->prev_pos = m->pos;
    m->pos.x = x;
    m->pos.y = y;
}

void increase_cursor_size(mouse* m,int increment){
    if (m->cursor_size + increment > m->max_cursor_size || m->cursor_size + increment < m->min_cursor_size) return;
    m->cursor_size += increment;
}

/////////////////////////////////////////////////////////////////////////////////////////:


// STICKS AND POINTS


struct Stick;
struct Particle;

struct Stick {
    struct Particle* p1;
    struct Particle* p2;
    float length;
    bool is_active;
    bool is_selected;
};

struct Particle {
    struct Stick* s1;
    struct Stick* s2;
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



particle** create_particles(int startX,int startY,int rows,int columns,int spacing){
    particle** particles = malloc(sizeof(particle*) * rows);
    for (int i = 0; i < rows;i++){
        particles[i] = malloc(sizeof(particle) * columns);
        for (int j = 0; j < columns;j++){
            particle p = {.mass = 10,.x = startX + j * spacing,.y = startY + i * spacing};
            p.prevx = p.initx = p.x;
            p.prevy = p.inity = p.y;
            p.is_pinned = false;
            p.is_selected = false;
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
}


void update_particle(particle* p,float dt,float drag, vect2 acceleration,float elasticity,mouse* m){
    vect2 mouse_pos = m->pos;
    vect2 cursorToPosDir = {.x = p->x - mouse_pos.x,.y = p->y - mouse_pos.y};
    float cursorToPosDist = cursorToPosDir.x * cursorToPosDir.x + cursorToPosDir.y * cursorToPosDir.y;
    p->is_selected = cursorToPosDist < m->cursor_size * m->cursor_size;
    
    if (p->s1 != NULL) p->s1->is_selected = p->is_selected; 
    if (p->s2 != NULL) p->s2->is_selected = p->is_selected;
    

    if (m->left_button_down && p->is_selected){
        vect2 difference = diff(m->pos,m->prev_pos);
        if (difference.x > elasticity) difference.x = elasticity;
        if (difference.x < -elasticity) difference.x = -elasticity;
        if (difference.y > elasticity) difference.y = elasticity;
        if (difference.y < -elasticity) difference.y = -elasticity;
        p->prevx = p->x - difference.x;
        p->prevy = p->y - difference.y;
    }

    if (m->right_button_down && p->is_selected){ //tearing
        if (p->s1 != NULL) p->s1->is_active = false;
        if (p->s2 != NULL) p->s2->is_active = false;
    }


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
    keep_inside_view(p,SCREEN_WIDTH,SCREEN_HEIGHT);
}

///////////////////////////////////////////////////////////////////////////:::

struct Cloth {
    vect2 acceleration;
    float drag;
    float elasticity;
    particle** particles;
    int rows;
    int columns;
    stick** sticks;
    int nb_sticks;
};

typedef struct Cloth cloth;


cloth* cloth_new(float drag,float elasticity,int rows,int columns){
    cloth* c = malloc(sizeof(cloth));
    vect2 acceleration = {.x = 0.0f,.y = 981.0f};
    c->acceleration = acceleration;
    c->drag = drag;
    c->elasticity = elasticity;
    c->rows = rows;
    c->columns = columns;
    c->particles = create_particles(100,100,rows,columns,SPACING);
    int nb_sticks;
    c->sticks = create_sticks(c->particles,rows,columns,&nb_sticks);
    c->nb_sticks = nb_sticks;
    return c;
}
//Cloth 

void cloth_update(cloth* c,mouse* m){
    float dt = 0.016f;
    c->acceleration.x *= 0.8f;
    for (int i = 0;i < c->rows;i++){
        for (int j = 0; j < c->columns;j++){
            update_particle(&c->particles[i][j],dt,c->drag,c->acceleration,c->elasticity,m);
        }
    }
    for (int i = 0; i < c->nb_sticks;i++){
        update_stick(c->sticks[i]);
    }
}

void cloth_draw(SDL_Renderer* renderer,cloth* c){
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    for (int i = 0; i < c->nb_sticks;i++){
        if (c->sticks[i]->is_selected) SDL_SetRenderDrawColor(renderer,255,0,0,255);
        else SDL_SetRenderDrawColor(renderer,0,0,0,255);
        draw_stick(renderer,c->sticks[i]);
    }
}


void cloth_delete(cloth* c){
    for (int i = 0; i < c->nb_sticks;i++){
        free(c->sticks[i]);
    }
    for (int i = 0; i < c->rows;i++){
        free(c->particles[i]);
    }
    free(c->sticks);
    free(c->particles);
    free(c);
}


//////////////////////////////////////////////////////////////////////////:::

struct Application {
    SDL_Renderer* renderer;
    cloth* c;
    mouse* m;
};

typedef struct Application application;

application* application_create(SDL_Renderer* renderer,float drag,float elasticity,int rows,int columns){
    application* app = malloc(sizeof(application));
    app ->c = cloth_new(drag,elasticity,rows,columns);
    app->m = mouse_create(0,0);
    app->renderer = renderer;
    return app;
}

void application_delete(application* app){
    cloth_delete(app->c);
    free(app->m);
    free(app);
}

void application_update(application* app){
    cloth_update(app->c,app->m);
}

void application_render(application* app){
    SDL_SetRenderDrawColor(app->renderer,255,255,255,255);
    SDL_RenderClear(app->renderer);
    SDL_SetRenderDrawColor(app->renderer,0,0,0,255);
    cloth_draw(app->renderer,app->c);
    SDL_RenderPresent(app->renderer);
}

//////////////////////////////////////////////////////////////////////////////////////////

int events_handling(application* app,mouse* m){
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        switch (e.type)
        {
        case SDL_QUIT:
            return 1;
        
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym){
            case SDLK_ESCAPE:
                return 1;
            
            case SDLK_LEFT:
                app->c->acceleration.x = -10000;
                break;
            
            case SDLK_RIGHT:
                app->c->acceleration.x = 10000;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEMOTION:{
                int x = e.motion.x;
                int y = e.motion.y;
                m->pos.x = x;
                m->pos.y = y;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:{
                int x,y;
                SDL_GetMouseState(&x,&y);
                update_pos(m,x,y);
                if (!m->left_button_down && e.button.button == SDL_BUTTON_LEFT) m->left_button_down = true;
                if (!m->right_button_down && e.button.button == SDL_BUTTON_RIGHT) {
                    m->right_button_down = true;
                }
            }
            break;
        
        case SDL_MOUSEBUTTONUP:{
                if (m->left_button_down && e.button.button == SDL_BUTTON_LEFT) m->left_button_down = false;
                if (m->right_button_down && e.button.button == SDL_BUTTON_RIGHT) m->right_button_down = false;
            }
            break;
        case SDL_MOUSEWHEEL:
            if (e.wheel.y > 0) increase_cursor_size(m,10);
            else if (e.wheel.y < 0) increase_cursor_size(m,-10);
            break;
        default:
            break;
        }
    }
    return 0;
}


int main(int argc, char* argv[]){
    if (argc != 3) return EXIT_FAILURE;
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,SCREEN_WIDTH,SCREEN_HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;
    application* app = application_create(renderer,0.01f,10.0f,rows,columns);
    while (true){
        if (events_handling(app,app->m) == 1) break;
        application_update(app);
        application_render(app);
        SDL_Delay(16);
    }
    application_delete(app);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

//gcc particle.c -o particle -Wall -Wvla -Wextra -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm