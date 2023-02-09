#include "../include/stick.h"


stick* create_sticks(particle* particles,int n){
    stick* sticks = malloc(sizeof(stick) * (n + 2));
    for (int i = 0; i < n;i++){
        int next = (i + 1) % n;
        stick s = {.p1 = &particles[i],.p2 = &particles[next],.length = dist(particles[i],particles[next])};
        sticks[i] = s;
    }
    stick s = {.p1 = &particles[0],.p2 = &particles[2],.length = dist(particles[0],particles[2])};
    sticks[4] = s;
    stick s1 = {.p1 = &particles[1],.p2 = &particles[3],.length = dist(particles[1],particles[3])};
    sticks[5] = s1;
    return sticks;
}

void update_sticks(stick* sticks,int n){
    for (int i = 0; i < stick_len;i++){
        vect2 diff = get_diff(sticks[i].p1,sticks[i].p2);
        float diff_factor = 0.5 * (sticks[i].length - get_length(diff)) / get_length(diff);
        vect2 offset = {.x = diff.x * diff_factor,.y = diff.y * diff_factor};
        sticks[i].p1->x += offset.x;
        sticks[i].p1->y += offset.y;
        sticks[i].p2->x -= offset.x;
        sticks[i].p2->y -= offset.y;
    }
}