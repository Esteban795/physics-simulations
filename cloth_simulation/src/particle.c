#include "../include/particle.h"


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
    printf("position : (%.3f,%.3f), prev position : (%.3f,%.3f)\n",p.x,p.y,p.prevx,p.prevy);
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
            p.prevx = p.x;
            p.prevy = p.y;
            p.is_pinned = false;
            p.is_selected = false;
            particles[i][j] = p;
        }
    }
    return particles;
}

void add_stick(particle* p,stick s,int index){
    p->sticks[index] = s;
}

void update_particle(particle* p,float dt,float drag, vect2 acceleration,float elasticity,mouse* m,int width,int height){
    vect2 mouse_pos = mouse_get_pos(m);
    vect2 cursorToPosDir = {.x = p->x - mouse_pos.x,.y = p->y - mouse_pos.y};
    p->is_selected = vector_magnitude(divide(cursorToPosDir,m->cursor_size * m->cursor_size)) < 1 ? true : false;
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
