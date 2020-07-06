#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <sdl_circle.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define GROUND_Y SCREEN_HEIGHT * 0.75

typedef struct Rocket Rocket;
struct Rocket {
    int rocketX;
    int rocketY;
};

void drawWorld(SDL_Renderer *renderer, Rocket* Rocket);
void drawCycle(SDL_Renderer* renderer, Rocket* Rocket);
void drawRock(SDL_Renderer* renderer, Rocket* Rocket);