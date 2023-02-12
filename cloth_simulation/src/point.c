#include "../include/point.h"


/*
Get euclidean distance between two points.
*/

/*
Returns a vect2 with difference of x and y
*/
vect2 get_diff(point* p1,point* p2){
    vect2 temp = {.x = p1->x - p2->x,.y = p1->y - p2->y};
    return temp;
}

void print_point(point p){
    printf("position : (%.3f,%.3f), prev position : (%.3f,%.3f), pinned : %d\n",p.x,p.y,p.prevx,p.prevy,p.is_pinned);
}



point** create_points(int startX,int startY,int rows,int columns,int spacing){
    point** points = malloc(sizeof(point*) * rows);
    for (int i = 0; i < rows;i++){
        points[i] = malloc(sizeof(point) * columns);
        for (int j = 0; j < columns;j++){
            point p = {.mass = 10,.x = startX + j * spacing,.y = startY + i * spacing};
            p.prevx = p.initx = p.x;
            p.prevy = p.inity = p.y;
            p.is_pinned = false;
            p.is_selected = false;
            points[i][j] = p;
        }
    }
    return points;
}

void keep_inside_view(point* p,int width,int height){
    if (p->x > width){
        p->x = width;
        p->prevx = p->x;
    }
    if (p->x < 0){
        p->x = 0;
        p->prevx = p->x;
    }
    if (p->y > height){
        p->y = height;
        p->prevy = p->y;
    }
}


void update_point(point* p,float dt,float drag, vect2 acceleration,float elasticity,mouse* m,const int SCREEN_WIDTH,const int SCREEN_HEIGHT){
    vect2 mouse_pos = m->pos;
    vect2 cursorToPosDir = {.x = p->x - mouse_pos.x,.y = p->y - mouse_pos.y};
    float cursorToPosDist = cursorToPosDir.x * cursorToPosDir.x + cursorToPosDir.y * cursorToPosDir.y;
    p->is_selected = cursorToPosDist < m->cursor_size * m->cursor_size;
    
    if (p->s1 != NULL) p->s1->is_selected = p->is_selected; 
    if (p->s2 != NULL) p->s2->is_selected = p->is_selected;
    

    if (m->left_button_down && p->is_selected){
        vect2 difference = diff(m->pos,m->prev_pos);
        if (difference.x > elasticity) difference.x = elasticity;
        if (difference.x < -elasticity) difference.x = -elasticity;
        if (difference.y > elasticity) difference.y = elasticity;
        if (difference.y < -elasticity) difference.y = -elasticity;
        p->prevx = p->x - difference.x;
        p->prevy = p->y - difference.y;
    }

    if (m->right_button_down && p->is_selected){ //tearing
        if (p->s1 != NULL) p->s1->is_active = false;
        if (p->s2 != NULL) p->s2->is_active = false;
    }


    if (p->is_pinned){
        p->x = p->initx;
        p->y = p->inity;
        return;
    }
    float new_x = p->x + (p->x - p->prevx) * (1.0f - drag) + acceleration.x * (1.0f - drag) * dt * dt;
    float new_y = p->y + (p->y - p->prevy) * (1.0f - drag) + acceleration.y * (1.0f - drag) * dt * dt;
    p->prevx = p->x;
    p->prevy = p->y;
    p->x = new_x;
    p->y = new_y;
    keep_inside_view(p,SCREEN_WIDTH,SCREEN_HEIGHT);
}