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
}