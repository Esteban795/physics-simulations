#include "../include/engine.h"



application* new_app(SDL_Renderer* renderer,int width,int height,int startX, int startY){
    application* app = malloc(sizeof(application));
    app->c = new_cloth(width,height,SPACING,startX,startY);
    app->last_update_time = 0;
    app->m = mouse_create(startX,startY);
    app->renderer  = renderer;
    return app;
}

void update(application* app){
    uint32_t current_time = SDL_GetTicks();
    float dt = (current_time - app->last_update_time) / 1000.0f;
    cloth_update(app->c,app->m,dt);
    app->last_update_time = current_time;
}

void render(application* app){
    SDL_SetRenderDrawColor(app->renderer,255,255,255,255);
    SDL_RenderClear(app->renderer);
    SDL_SetRenderDrawColor(app->renderer,0,0,0,255);
    for (int i = 0; i < app->c->rows;i++){
        for (int j = 0; j < app->c->columns;j++){
            DrawCircle(app->renderer,app->c->particles[i][j].x,app->c->particles[i][j].y,20);
        }
    }
    cloth_draw(app->c,app->renderer);
    SDL_RenderPresent(app->renderer);
}

void destroy(application* app){
    cloth_delete(app->c);
    free(app->m);
    free(app);
}

int main_loop(SDL_Renderer* renderer,int width,int height){
    application* app = new_app(renderer,width,height,100,100);
    SDL_Event e;
    int running = 1;
    render(app);
    SDL_Delay(2000);
    update(app);
    render(app);
    SDL_Delay(2000);
    update(app);
    render(app);
    while (running){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                    case SDLK_q:
                        running = 0;
                        break;
                }
            }
        }
        SDL_Delay(16);
    }
    destroy(app);
    return 0;
}