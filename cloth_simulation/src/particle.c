#include "../include/particle.h"




float dist(particle p1,particle p2){
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

vect2 get_diff(particle* p1,particle* p2){
    vect2 temp = {.x = p1->x - p2->x,.y = p1->y - p2->y};
    return temp;
}

void print_particle(particle p){
    printf("position : (%.3f,%.3f), prev position : (%.3f,%.3f)\n",p.x,p.y,p.prevx,p.prevy);
}


particle* create_particles(int n){
    particle* particles = malloc(sizeof(particle) * n);
    for (int i = 0; i < n;i++){
        float x = rand_between(10,600);
        float y = rand_between(10,600);
        particle p = {.x = x,.y = y,.prevx = x,.prevy = y,.mass = 100};
        particles[i] = p;
    }
    return particles;
}

void update_particles(particle* particles,int n){
    for (int i = 0; i < n;i++){
        vect2 force = {.x = 0,.y = 9.81};
        vect2 acceleration = {.x = force.x / particles[i].mass,.y = force.y /particles[i].mass};
        vect2 prev_pos = {.x = particles[i].x,.y = particles[i].y};
        particles[i].x = 2 * particles[i].x - particles[i].prevx + acceleration.x * dt * dt;
        particles[i].y = 2 * particles[i].y - particles[i].prevy + acceleration.y * dt * dt;

        particles[i].prevx = prev_pos.x;
        particles[i].prevy = prev_pos.y;
        keep_inside_view(&particles[i]);
    }
}

particle* predefinies(){
    particle* particles = malloc(sizeof(particle) * 4);
    particles[0].x = 220;
    particles[0].y = 120;
    particles[0].mass = 10;
    particles[0].prevx = particles[0].x;
    particles[0].prevy = particles[0].y;
    particles[1].x = 420;
    particles[1].y = 120;
    particles[1].mass = 10;
    particles[1].prevx = particles[1].x;
    particles[1].prevy = particles[1].y;
    particles[2].x = 420;
    particles[2].y = 320;
    particles[2].mass = 10;
    particles[2].prevx = particles[2].x;
    particles[2].prevy = particles[2].y;
    particles[3].x = 220;
    particles[3].y = 320;
    particles[3].mass = 10;
    particles[3].prevx = particles[3].x;
    particles[3].prevy = particles[3].y;
    return particles;
}