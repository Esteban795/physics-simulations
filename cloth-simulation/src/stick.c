#include "../include/stick.h"

/*
Print stick infos, mainly used for debugging purposes
*/
void print_stick(stick s){
    printf("P1 (%f,%f),P2 : (%f,%f), length : %f, is_active : %d\n",s.p1->x,s.p1->y,s.p2->x,s.p2->y,s.length,s.is_active);
}

/*
Counts the number of sticks needed for the creation of the sticks array.
There probably is a math formula for that, but I'm too lazy to work this out right now.
*/
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

/*
Creates an array of pointers to sticks, each stick with is_active field at true by default.
Each stick follows this pattern :
- p1 and p2 are both points, that are at each end of the stick. (Left from our current point, and above)
- The length is the spacing

It also pins each even point that is on the first row, to avoid the cloth to just drop down and be useless
*/
stick** create_sticks(point** points,int rows,int columns,int* nb_sticks){
    *nb_sticks = get_nb_sticks(rows,columns);
    stick** sticks = malloc(sizeof(stick*) * *nb_sticks); //That's why we need the number of points, no dynamic arrays.
    int index = 0;
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (j != 0){
                stick* s = create_new_stick(&points[i][j],&points[i][j - 1]); // Stick between this point and left point from this one
                points[i][j].s1 = s;
                points[i][j - 1].s1 = s;
                sticks[index] = s;
                index++;
            }
            if (i != 0){
                stick* s = create_new_stick(&points[i][j],&points[i - 1][j]); // Same, but with the above point
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

/*
Updates the stick to apply the maintain the given constraints and so moves around the particle as much as diff_factor (Hooke's law basically) needs it.
*/
void update_stick(stick* s){
    if (!s->is_active) return; 

    vect2 p1_pos = {.x = s->p1->x , .y = s->p1->y};
    vect2 p2_pos = {.x = s->p2->x, .y = s->p2->y};

    vect2 difference = diff(p1_pos,p2_pos);
    
    float distance = sqrtf(difference.x * difference.x + difference.y * difference.y);
    float diff_factor = (s->length - distance) / distance;

    vect2 offset = {.x = difference.x * diff_factor * 0.5f, .y = difference.y * diff_factor * 0.5f};

    if (!s->p1->is_pinned){ //If a point is pinned, it shouldn't move. So we don't apply anything to it.
        s->p1->x = p1_pos.x + offset.x;
        s->p1->y = p1_pos.y + offset.y;
    }
    if (!s->p2->is_pinned){
        s->p2->x = p2_pos.x - offset.x;
        s->p2->y = p2_pos.y - offset.y;
    }
}

//Do I really need to explain this one ?
void draw_stick(SDL_Renderer* renderer,stick* s){
    if (!s->is_active) return;
    SDL_RenderDrawLine(renderer,s->p1->x,s->p1->y,s->p2->x,s->p2->y);
}
