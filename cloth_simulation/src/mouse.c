#include "../include/mouse.h"


mouse* mouse_create(float x,float y){
    mouse* m = malloc(sizeof(mouse));
    vect2 pos = {.x = x,.y = y};
    m->pos = pos;
    m->prev_pos = pos;
    m->cursor_size = 20;
    m->left_button_down = false;
    m->right_button_down = false;
    m->max_cursor_size = 100;
    m->min_cursor_size = 20;
    return m;
}

vect2 mouse_get_pos(mouse* m){
    return m->pos;
}

vect2 mouse_get_prevpos(mouse* m){
    return m->prev_pos;
}

vect2 mouse_diff_pos(mouse* m){
    return diff(m->pos,m->prev_pos);
}

void update_pos(mouse* m,int x,int y){
    m->prev_pos = m->pos;
    m->pos.x = x;
    m->pos.y = y;
}

void increase_cursor_size(mouse* m,int increment){
    if (m->cursor_size + increment > m->max_cursor_size || m->cursor_size + increment < m->min_cursor_size) return;
    m->cursor_size += increment;
}