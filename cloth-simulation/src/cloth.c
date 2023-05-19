#include "../include/cloth.h"


/*
The actual cloth structure, which regroups both points, sticks and acceleration that needs to be applied to the cloth.
There are several other parameters, but you can easily deduce what they do.
*/
cloth* cloth_new(float drag,float elasticity,int rows,int columns){
    cloth* c = malloc(sizeof(cloth));
    vect2 acceleration = {.x = 0.0f,.y = 981.0f};
    c->acceleration = acceleration;
    c->drag = drag;
    c->elasticity = elasticity;
    c->rows = rows;
    c->columns = columns;
    c->points = create_points(100,100,rows,columns,SPACING);
    int nb_sticks;
    c->sticks = create_sticks(c->points,rows,columns,&nb_sticks);
    c->nb_sticks = nb_sticks;
    return c;
}


void cloth_update(cloth* c,mouse* m){
    float dt = 0.016f;
    c->acceleration.x *= 0.8f; // to diminish x acceleration due to wind impulse (left or right arrow)
    for (int i = 0;i < c->rows;i++){
        for (int j = 0; j < c->columns;j++){
            update_point(&c->points[i][j],dt,c->drag,c->acceleration,c->elasticity,m,1200,700);
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
        free(c->points[i]);
    }
    free(c->sticks);
    free(c->points);
    free(c);
}