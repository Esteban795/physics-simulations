#include "../include/cloth.h"


cloth* new_cloth(int width,int height, int spacing, int startX,int startY){
    cloth* c = malloc(sizeof(cloth));
    c->drag = 0.01f;
    c->elasticity = 10.0f;
    vect2 gravity = {.x = 0.0,.y = 981.0};
    c->gravity = gravity;
    vect2 dimensions;
    c->particles = create_particles(startX,startY,width,height,spacing,&dimensions);
    c->rows = dimensions.y;
    c->columns = dimensions.x;
    int nb_sticks;
    c->sticks = 
}