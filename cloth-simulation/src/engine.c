#include "../include/engine.h"


/*
Events handling part. Returning 1 means we wanna quit.
*/
int events_handling(application* app,mouse* m){
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        switch (e.type)
        {
        case SDL_QUIT:
            return 1;
        
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym){
            case SDLK_ESCAPE:
                return 1;
            
            case SDLK_LEFT: //The kind of wind impulse I came up with (it sucks but I have no clue how to do better)
                app->c->acceleration.x = -10000;
                break;
            
            case SDLK_RIGHT:
                app->c->acceleration.x = 10000;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEMOTION:{ //updating mouse position
                int x = e.motion.x;
                int y = e.motion.y;
                m->pos.x = x;
                m->pos.y = y;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:{
                int x,y;
                SDL_GetMouseState(&x,&y);
                update_pos(m,x,y);
                if (!m->left_button_down && e.button.button == SDL_BUTTON_LEFT) m->left_button_down = true;
                if (!m->right_button_down && e.button.button == SDL_BUTTON_RIGHT) m->right_button_down = true;
            }
            break;
        
        case SDL_MOUSEBUTTONUP:{
                if (m->left_button_down && e.button.button == SDL_BUTTON_LEFT) m->left_button_down = false;
                if (m->right_button_down && e.button.button == SDL_BUTTON_RIGHT) m->right_button_down = false;
            }
            break;
        case SDL_MOUSEWHEEL:
            if (e.wheel.y > 0) increase_cursor_size(m,10);
            else if (e.wheel.y < 0) increase_cursor_size(m,-10);
            break;
        default:
            break;
        }
    }
    return 0;
}

/*
A nice little wrapper to not have 16 damn arguments in each function.
*/
application* application_create(SDL_Renderer* renderer,float drag,float elasticity,int rows,int columns){
    application* app = malloc(sizeof(application));
    app ->c = cloth_new(drag,elasticity,rows,columns);
    app->m = mouse_create(0,0);
    app->renderer = renderer;
    return app;
}

void application_delete(application* app){
    cloth_delete(app->c);
    free(app->m);
    free(app);
}

void application_update(application* app){
    cloth_update(app->c,app->m);
}

void application_render(application* app){
    SDL_SetRenderDrawColor(app->renderer,255,255,255,255);
    SDL_RenderClear(app->renderer);
    SDL_SetRenderDrawColor(app->renderer,0,0,0,255);
    cloth_draw(app->renderer,app->c);
    SDL_RenderPresent(app->renderer);
}

/*
See why I made a wrapper ?
*/
int main_loop(SDL_Renderer* renderer,int rows,int columns){
    application* app = application_create(renderer,0.015f,30.0f,rows,columns);
    while (true){
        if (events_handling(app,app->m) == 1) break;
        application_update(app);
        application_render(app);
        SDL_Delay(16);
    }
    application_delete(app);
    return 0;
}