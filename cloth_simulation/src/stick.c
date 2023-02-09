#include "../include/stick.h"


int get_nb_sticks(particle* particles,int rows,int columns){
    int count = 0;
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (i != 0) count += 1;
            if (j != 0) count += 1;
        }
    }
}

stick create_new_stick(particle* p1,particle* p2){
    stick s = {.p1 = p1,.p2 = p2,.length = dist(*p1,*p2),.is_active = true,.is_selected = false,.is_null = false};
    return s;
}

void create_sticks(particle** particles,int rows,int columns,int* nb_sticks){
    *nb_sticks = get_nb_sticks(particles,rows,columns);
    
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (j != 0){
                stick s = create_new_stick(&particles[i][j],&particles[i][j - 1]); //left particle
                particles[i][j - 1].sticks[0] = s;
                particles[i][j].sticks[0] = s;
            }
            if (i != 0){
                stick s = create_new_stick(&particles[i][j],&particles[i - 1][j]); //Top particle
                particles[i - 1][j].sticks[0] = s;
                particles[i][j].sticks[0] = s;
            }
            if (i == 0 && j % 2 == 0){
                particles[i][j].is_pinned = true;
            }
        }
    }
}

void update_sticks(stick* sticks,int stick_len){
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