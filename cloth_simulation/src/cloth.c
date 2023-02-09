#include "../include/cloth.h"



cloth* new_cloth(int width,int height, int spacing, int startX,int startY){
    cloth* c = malloc(sizeof(cloth));
    c->drag = 0.01f;
    c->elasticity = 10.0f;
    vect2 gravity = {.x = 0.0f,.y = 981.0f};
    c->gravity = gravity;
    c->particles = create_particles(startX,startY,width,height,spacing);
    c->rows = height;
    c->columns = width;
    int nb_sticks;
    c->sticks = create_sticks(c->particles,c->rows,c->columns,&nb_sticks);
    c->nb_sticks = nb_sticks;
    return c;
}


void cloth_update(cloth* c,mouse* m,float dt){
    for (int i = 0; i < c->rows;i++){
        for (int j = 0; j < c->columns;j++){
            update_particle(&c->particles[i][j],dt,c->drag,c->gravity,c->elasticity,m,c->rows,c->columns);
        }
    }
    for (int i = 0; i < c->nb_sticks;i++){
        update_stick(&c->sticks[i]);
    }
}

void cloth_draw(cloth* c,SDL_Renderer* renderer){
    for (int i = 0; i < c->nb_sticks;i++){
        draw_stick(renderer,c->sticks[i]);
    }
    SDL_RenderPresent(renderer);
}

void cloth_delete(cloth* c){
    for (int i = 0; i < c->rows;i++){
        for (int j = 0; j < c->columns;j++){
            free(c->particles[i][j].sticks);
        }
        free(c->particles[i]);
    }
    free(c->particles);
    free(c->sticks);
    free(c);
}