#include "../include/engine.h"

bool GRAVITY_ON = true;

void update_ball(ball* b){
    b->x += b->velX;
    b->y += b->velY;
    if (GRAVITY_ON) b->velY += GRAVITY;
}


bool check_ground_collision(ball* b){
    if (b->y + b->radius > HEIGHT){
        b->y = HEIGHT - b->radius;
        b->velY *= GROUND_BOUNCINESS;
        return true;
    } else if (b->y - b->radius < 0){
        b->y = b->radius;
        b->velY *= -GROUND_BOUNCINESS;
    }
    return false;
}

bool check_wall_collision(ball* b){
    if (b->x + b->radius > WIDTH){
        b->x = WIDTH - b->radius;
        return true;
    } else if (b->x - b->radius < 0){
        b->x = b->radius;
        return true;
    }
    return false;
}


bool check_ball_x_collision(ball* b1,ball* b2){
    return abs_float(b1->x - b2->x) < b1->radius + b2->radius;
}

bool check_ball_y_collision(ball* b1,ball* b2){
    return abs_float(b1->y - b2->y) < b1->radius + b2->radius;
}


void do_elastic_collision(ball* b1,ball* b2){
    //Magnitude is very close to zero (actually is zero, but floating points being floating points)
    float magnitude_vel_b1 = velocity_magnitude(b1);
    float magnitude_vel_b2 = velocity_magnitude(b2);
    if (magnitude_vel_b1 < 1e-5 && magnitude_vel_b2 < 1e-5) return;

    //Normal vector to collision surface
    vect2 v_n = {.x= b2->x - b1->x,.y = b2->y - b1->y};
    float v_n_magnitude = vector_magnitude(v_n);
    vect2 v_un = {.x = v_n.x / v_n_magnitude,.y = v_n.y/ v_n_magnitude}; //On le normalise

    //Vector tangent to collision surface
    vect2 v_ut = {.x= -v_un.y,.y=v_un.x};

    vect2 vel1 = {.x=b1->velX,.y = b1->velY};
    vect2 vel2 = {.x=b2->velX,.y = b2->velY};

    double v1n = dot_product(v_un,vel1);
    double v1t = dot_product(v_ut,vel1);
    double v2n = dot_product(v_un,vel2);
    double v2t = dot_product(v_ut,vel2);

    double v1n_f = (v1n * (b1->mass - b2->mass) + 2. * b2->mass * v2n) / (b1->mass + b2->mass);
    double v2n_f = (v2n * (b2->mass - b1->mass) + 2. * b1->mass * v1n) / (b1->mass + b2->mass);

    vect2 v_v1nprime = {.x= v1n_f * v_un.x,.y = v1n_f * v_un.y};
    vect2 v_v1tPrime = {.x= v1t * v_ut.x,.y =v1t * v_ut.y};
	vect2  v_v2nPrime = {.x= v2n_f * v_un.x,.y = v2n_f * v_un.y};
	vect2  v_v2tPrime = {.x= v2t * v_ut.x,.y =v2t * v_ut.y};

    b1->velX = v_v1nprime.x + v_v1tPrime.x;
    b1->velY = v_v1nprime.y + v_v1tPrime.y;
    b2->velX = v_v2nPrime.x + v_v2tPrime.x;
    b2->velY = v_v2nPrime.y + v_v2tPrime.y;
}

void main_loop(SDL_Renderer* renderer,int NB_BALLS){
    SDL_Event e;
    ball** tab_balls = malloc(sizeof(ball*) * NB_BALLS);
    int running = 1;
    for (int i = 0; i < NB_BALLS;i++){
        float velx = rand_between(0,20) - 10;
        float vely = rand_between(0,20) - 10;
        tab_balls[i] = create_ball(BALL_RADIUS + (i + 1) * WIDTH/10,HEIGHT/2,velx,vely,BALL_RADIUS,20);
    }

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    for (int i = 0; i < NB_BALLS;i++){
        DrawCircle(renderer,tab_balls[i]->x,tab_balls[i]->y,BALL_RADIUS);
    }
    SDL_RenderPresent(renderer);
    while (running){
        SDL_Delay(FRAME_DELAY);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);
        
        for (int i = 0; i < NB_BALLS;i++){
            update_ball(tab_balls[i]);
        }

        for (int i = 0; i < NB_BALLS;i++){
            if (check_ground_collision(tab_balls[i])){
                tab_balls[i]->velY *= -0.8;
            }
            if (check_wall_collision(tab_balls[i])) {
                tab_balls[i]->velX *= -WALL_BOUNCINESS;
            }
            for (int j = i + 1; j < NB_BALLS;j++){
                if (check_ball_y_collision(tab_balls[i],tab_balls[j]) && check_ball_x_collision(tab_balls[i],tab_balls[j])){
                    do_elastic_collision(tab_balls[i],tab_balls[j]);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        for (int i = 0; i < NB_BALLS;i++){
            DrawCircle(renderer,tab_balls[i]->x,tab_balls[i]->y,BALL_RADIUS);
        }
        SDL_RenderPresent(renderer);


        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                case SDLK_q:
                    running = 0;
                    break;
                case SDLK_g:
                    GRAVITY_ON = !GRAVITY_ON;
                    break;
                case SDLK_r:
                    for (int i = 0; i < NB_BALLS;i++){
                        tab_balls[i]->x = rand_between(BALL_RADIUS,WIDTH - BALL_RADIUS);
                        tab_balls[i]->y = rand_between(BALL_RADIUS,HEIGHT - BALL_RADIUS);
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    for (int i = 0; i < NB_BALLS;i++){
        free(tab_balls[i]);
    }
    free(tab_balls);
}