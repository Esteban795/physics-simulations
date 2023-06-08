#include "../include/spring.h"



int get_nb_springs(int rows,int columns){
    int count = 0;
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (i != 0) count += 1;
            if (j != 0) count += 1;
            if (i != 0 && j != 0) count++;
            if (i != 0 && j < rows - 1) count++;
        }
    }
    return count;
}

spring* create_new_spring(point* p1,point* p2,double stiff,double damp){
    spring* s = malloc(sizeof(spring));
    s->p1 = p1;
    s->p2 = p2;
    s->length = vect2_length(vect2_diff(p1->pos,p2->pos));
    s->stiffness = stiff;
    s->damping = damp;
    return s;
}

spring** create_springs(point** points,int rows,int columns,int* nb_springs){
    *nb_springs = get_nb_springs(rows,columns);
    spring** springs = malloc(sizeof(spring*) * *nb_springs);
    int index = 0;
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (i != 0 && j != columns - 1){
                spring* s = create_new_spring(&points[i][j],&points[i - 1][j + 1],RIGID_STIFFNESS,RIGID_DAMPING); //top right point
                springs[index] = s;
                index++;
            }
            if (i != 0 && j != 0){
                spring* s = create_new_spring(&points[i][j],&points[i - 1][j - 1],RIGID_STIFFNESS,RIGID_DAMPING); //top-left point
                springs[index] = s;
                index++;
            }
            if (j != 0){
                spring* s = create_new_spring(&points[i][j],&points[i][j - 1],RIGID_STIFFNESS,RIGID_DAMPING); //left point
                springs[index] = s;
                index++;
            }
            if (i != 0){
                spring* s = create_new_spring(&points[i][j],&points[i - 1][j],RIGID_STIFFNESS,RIGID_DAMPING); //Top point
                springs[index] = s;
                index++;
            }
        }
    }
    return springs;
}


void update_spring(spring* s){
    //Force should be F = - stiffness * ((p1_pos - p0_pos) - s->length) + k_damper * (v1 - v0)
    
    vect2 pos_diff = vect2_diff(s->p1->pos,s->p2->pos);
    vect2 vel_diff = vect2_diff(s->p1->vel,s->p2->vel);
    
    float distance = vect2_length(pos_diff);
    double coeff =  - (s->stiffness * (distance - s->length) + s->damping * dot_product(vel_diff,pos_diff) / distance);
    
    vect2 f1 = vect2_multiply(pos_diff,coeff / distance);
    vect2 f2 = vect2_multiply(f1,-1);

    if (!s->p1->is_fixed){ //If a point is pinned, it shouldn't move. So we don't apply anything to it.
        apply_force(s->p1,f1);
    }
    if (!s->p2->is_fixed){
        apply_force(s->p2,f2);
    }
    
}

void delete_springs(spring** springs,int nb_springs){
    for (int i = 0; i < nb_springs; i++){
        free(springs[i]);
    }
    free(springs);
}