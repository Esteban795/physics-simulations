#include "../include/particle.h"


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
                particles[i][j - 1].sticks[0] = s;
                particles[i][j].sticks[0] = s;
                sticks[index] = s;
                index++;
            }
            if (i != 0){
                stick s = create_new_stick(&particles[i][j],&particles[i - 1][j]); //Top particle
                particles[i - 1][j].sticks[0] = s;
                particles[i][j].sticks[0] = s;
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
    vect2 p1 = {.x = s->p1->x,.y = s->p1->y};
    vect2 p2 = {.x = s->p2->x,.y = s->p2->y};

    vect2 difference = diff(p1,p2);
    float dist = get_length(difference);
    float diff_factor = 0.5 * (s->length - dist)/dist;
    vect2 offset = {.x = difference.x * diff_factor,.y = difference.y * diff_factor};

    s->p1->x = p1.x + offset.x;
    s->p1->y = p1.y + offset.y;

    s->p2->x = p2.x + offset.x;
    s->p2->y = p2.y + offset.y;
}

void draw_stick(SDL_Renderer* renderer,stick* s){
    if (!s->is_active) return;
    SDL_RenderDrawLine(renderer,s->p1->x,s->p1->y,s->p2->x,s->p2->y);
}

//gcc stick.c -o stick -Wall -Wvla -Wextra -fsanitize=address
/*
Get euclidean distance between two particles.
*/
float dist(particle p1,particle p2){
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

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



particle** create_particles(int startX,int startY,int width,int height,int spacing,vect2* dimensions){
    int p = width/spacing;
    int n = height/spacing;
    dimensions->x = p;
    dimensions->y = n;
    particle** particles = malloc(sizeof(particle*) * n);
    for (int i = 0; i < n;i++){
        particles[i] = malloc(sizeof(particle) * p);
        for (int j = 0; j < p;j++){
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

void add_stick(particle* p,stick s,int index){
    p->sticks[index] = s;
}

void update_particle(particle* p,float dt,float drag, vect2 acceleration,float elasticity,mouse* m,int width,int height){
    vect2 mouse_pos = m->pos;
    vect2 cursorToPosDir = {.x = p->x - mouse_pos.x,.y = p->y - mouse_pos.y};
    float cursorToPosDist = cursorToPosDir.x * cursorToPosDir.x + cursorToPosDir.y * cursorToPosDir.y;
    p->is_selected = cursorToPosDist < m->cursor_size * m->cursor_size;
    
    if (!p->sticks[0].is_null) p->sticks[0].is_selected = true;
    if (!p->sticks[1].is_null) p->sticks[1].is_selected = true;

    if (m->left_button_down && p->is_selected){
        vect2 diff = mouse_diff_pos(m);
        if (diff.x > elasticity) diff.x = elasticity;
        if (diff.y > elasticity) diff.y = elasticity;
        if (diff.x < -elasticity) diff.x = -elasticity;
        if (diff.y < -elasticity) diff.y = -elasticity;

        p->prevx = p->x - diff.x;
        p->prevy = p->y - diff.y;
    }
    if (m->right_button_down && p->is_selected){
        if (!p->sticks[0].is_null) p->sticks[0].is_active = false;
        if (!p->sticks[1].is_null) p->sticks[1].is_active = false;
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
    keep_inside_view(p,width,height);
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

int main(void){
    vect2 dim;
    particle** particles = create_particles(100,100,500,500,200,&dim);
    mouse* m = mouse_create(300,300);
    int rows = dim.y;
    int columns = dim.x;
    int nb_sticks;
    stick* sticks = create_sticks(particles,rows,columns,&nb_sticks);
    vect2 gravity = {.x = 0.0f,.y = 981.0f};
    printf("Initialement, les particules sont en  :\n");
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            print_particle(particles[i][j]);
        }
    }
    printf("\n Les sticks sont  : \n");
    for (int i = 0; i < nb_sticks;i++){
        print_stick(sticks[i]);
    }

    printf("On update les particules. \n\n");
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            update_particle(&particles[i][j],0.1,0.01,gravity,10.0f,m,600,600);
        }
    }
    printf("Après update des PARTICULES :\n");
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            print_particle(particles[i][j]);
        }
    }
    for (int i = 0; i < nb_sticks;i++){
        update_stick(&sticks[i]);
    }
    printf("\n\n\n\n\n\nApres update des sticks :\n");
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            print_particle(particles[i][j]);
        }
    }
    printf("\n\n");
    printf("\n Les sticks sont finalement : \n");
    for (int i = 0; i < nb_sticks;i++){
        print_stick(sticks[i]);
    }
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            free(particles[i][j].sticks);
        }
        free(particles[i]);
    }
    free(sticks);
    free(m);
    free(particles);
    return 0;
}

//gcc particle.c vect2.c mouse.c -o particle -Wall -Wvla -Wextra -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm