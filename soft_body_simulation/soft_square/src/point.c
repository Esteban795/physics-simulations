#include "../include/point.h"


void keep_inside_view(point* p,int width,int height){
    if (p->pos.x > width){
        
        p->pos.x = width;
        p->prev_pos.x = p->pos.x;
        
    }
    if (p->pos.x < 0.0){
        
        p->pos.x = 0.0;
        p->prev_pos.x = p->pos.x;
    }
    if (p->pos.y > height){
        p->pos.y = height;
        p->prev_pos.y = p->pos.y;
        
        
    }
}

point create_point(double x, double y, bool fixed,double mass){
    point p;
    vect2 pos = {.x = x, .y = y};
    p.pos = pos;
    p.prev_pos = pos;
    p.is_fixed = fixed;
    vect2 zero_vect = {.x = 0.0, .y = 0.0};
    p.force = zero_vect;
    p.mass = mass;
    p.vel = zero_vect;
    return p;
}


point** create_points(int startX,int startY,int rows,int columns,int spacing){
    point** points = malloc(sizeof(point*) * rows);
    for (int i = 0; i < rows;i++){
        points[i] = malloc(sizeof(point) * columns);
        for (int j = 0; j < columns;j++){
            points[i][j] = create_point(startX + j * spacing,startY + i * spacing,false,1.0);
        }
    }
    return points;
}


void apply_force(point* p,vect2 f){
    p->force = vect2_add(p->force,f);
}

void clear_forces(point** points,int rows,int columns){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns;j++){
            points[i][j].force.x *= 0.9;
            points[i][j].force.y = 50.0;
        }
    }
}

void update_pos(point* p,double dt){
    double new_x = 2 * p->pos.x - p->prev_pos.x + p->force.x * dt * dt / p->mass;
    double new_y = 2 * p->pos.y - p->prev_pos.y + (p->force.y * dt * dt / p->mass); 
    p->prev_pos = p->pos;
    p->pos.x = new_x;
    p->pos.y = new_y; 
}

//Verlet integration to update every points position
void update_positions(point** points,double dt,int rows, int columns){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            update_pos(&points[i][j],dt);
            keep_inside_view(&points[i][j],1024,768);
        }
    }
}   

void update_vel(point* p, double dt){
    p->vel.x = (p->pos.x - p->prev_pos.x)/dt;
    p->vel.y = (p->pos.y - p->prev_pos.y)/dt;
}

void update_velocities(point** points,double dt,int rows, int columns){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns;j++){
            update_vel(&points[i][j],dt);
        }
    }
}

void delete_points(point** points,int rows){
    for (int i = 0; i < rows;i++){
        free(points[i]);
    }
    free(points);
}