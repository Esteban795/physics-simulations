#include "../include/stick.h"

void print_stick(stick s){
    printf("P1 (%f,%f),P2 : (%f,%f), length : %f, is_active : %d\n",s.p1->x,s.p1->y,s.p2->x,s.p2->y,s.length,s.is_active);
}

int get_nb_sticks(int rows,int columns){
    int count = 0;
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (i != 0) count += 1;
            if (j != 0) count += 1;
        }
    }
    return count;
}

stick* create_new_stick(point* p1,point* p2){
    stick* s = malloc(sizeof(stick));
    s->p1 = p1;
    s->p2 = p2;
    s->length = SPACING;
    s->is_active = true;
    s->is_selected = false;
    return s;
}

stick** create_sticks(point** points,int rows,int columns,int* nb_sticks){
    *nb_sticks = get_nb_sticks(rows,columns);
    stick** sticks = malloc(sizeof(stick*) * *nb_sticks);
    int index = 0;
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (j != 0){
                stick* s = create_new_stick(&points[i][j],&points[i][j - 1]); //left particle
                points[i][j].s1 = s;
                points[i][j - 1].s1 = s;
                sticks[index] = s;
                index++;
            }
            if (i != 0){
                stick* s = create_new_stick(&points[i][j],&points[i - 1][j]); //Top particle
                points[i][j].s2 = s;
                points[i - 1][j].s2 = s;
                sticks[index] = s;
                index++;
            }
            if (i == 0 && j % 2 == 0){
                points[i][j].is_pinned = true;
            }
        }
    }
    return sticks;
}

void update_stick(stick* s){
    if (!s->is_active) return;

    vect2 p1_pos = {.x = s->p1->x , .y = s->p1->y};
    vect2 p2_pos = {.x = s->p2->x, .y = s->p2->y};

    vect2 difference = diff(p1_pos,p2_pos);
    
    float distance = sqrtf(difference.x * difference.x + difference.y * difference.y);
    float diff_factor = (s->length - distance) / distance;

    vect2 offset = {.x = difference.x * diff_factor * 0.5f, .y = difference.y * diff_factor * 0.5f};

    if (!s->p1->is_pinned){
        s->p1->x = p1_pos.x + offset.x;
        s->p1->y = p1_pos.y + offset.y;
    }
    if (!s->p2->is_pinned){
        s->p2->x = p2_pos.x - offset.x;
        s->p2->y = p2_pos.y - offset.y;
    }
}

void draw_stick(SDL_Renderer* renderer,stick* s){
    if (!s->is_active) return;
    SDL_RenderDrawLine(renderer,s->p1->x,s->p1->y,s->p2->x,s->p2->y);
}
