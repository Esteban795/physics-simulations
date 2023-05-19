#include "../include/point.h"

void print_point(point p){
    printf("position : (%.3f,%.3f), prev position : (%.3f,%.3f), pinned : %d\n",p.x,p.y,p.prevx,p.prevy,p.is_pinned);
}

/*
Creates a rows * columns matrix of points, that will be the main component of our cloth (even if they aren't actually drawn)
*/
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

/* 
To make sure the points don't get out of the window during the update_point function.
*/
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


/*
Verlet integration here.

First, we try to detect if the point are inside the cursor range.
If yes, we set is_selected field from sticks on true, and it will change their color when we will draw them.

Then, we try to to see if the user is trying to drag the cloth (only if the point is selected of course)
If yes, we just test if they dragged it so far that it should break. Both cases, we apply a "repulsive" ? restoring ? force that brings 
everything back in place, but smoothly.

The right click is the tearing part, without any drag. It is pretty straight-forward.

If it is pinned, the point doesn't move so we don't do anything.

Finally, we apply Verlet integration with a drag factor to not mess everything up.
*/
void update_point(point* p,float dt,float drag, vect2 acceleration,float elasticity,mouse* m,const int SCREEN_WIDTH,const int SCREEN_HEIGHT){
    vect2 mouse_pos = m->pos;
    vect2 cursorToPosDir = {.x = p->x - mouse_pos.x,.y = p->y - mouse_pos.y};
    float cursorToPosDist = cursorToPosDir.x * cursorToPosDir.x + cursorToPosDir.y * cursorToPosDir.y;
    p->is_selected = cursorToPosDist < m->cursor_size * m->cursor_size;
    
    if (p->s1 != NULL) p->s1->is_selected = p->is_selected; 
    if (p->s2 != NULL) p->s2->is_selected = p->is_selected;
    

    if (m->left_button_down && p->is_selected){
        vect2 difference = diff(m->pos,m->prev_pos);

        if (fabsf(difference.x) > elasticity || fabsf(difference.y) > elasticity){
            if (p->s1 != NULL) p->s1->is_active = false;
            if (p->s2 != NULL) p->s2->is_active = false;
        }
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
    float new_x = p->x + (p->x - p->prevx) * (1.0f - drag) + acceleration.x * (1.0f - drag) * dt * dt; // Verlet integration
    float new_y = p->y + (p->y - p->prevy) * (1.0f - drag) + acceleration.y * (1.0f - drag) * dt * dt;
    p->prevx = p->x;
    p->prevy = p->y;
    p->x = new_x;
    p->y = new_y;
    keep_inside_view(p,SCREEN_WIDTH,SCREEN_HEIGHT);
}