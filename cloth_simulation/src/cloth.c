#include "../include/cloth.h"



cloth* cloth_new(float drag,float elasticity,int rows,int columns){
    cloth* c = malloc(sizeof(cloth));
    vect2 acceleration = {.x = 0.0f,.y = 981.0f};
    c->acceleration = acceleration;
    c->drag = drag;
    c->elasticity = elasticity;
    c->rows = rows;
    c->columns = columns;
    c->particles = create_points(100,100,rows,columns,SPACING);
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
            update_point(&c->particles[i][j],dt,c->drag,c->acceleration,c->elasticity,m,1200,700);
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