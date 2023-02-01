#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>  

#define WIDTH 1500
#define HEIGHT 900
#define FRAME_DELAY 6

#define GRAVITY 0.1
#define WALL_BOUNCINESS 0.5
#define BALL_BOUNCINESS 0.9
#define GROUND_FRICTION 0.9
#define GROUND_BOUNCINESS 0.9
#define NB_BALLS 10

bool GRAVITY_ON = true;