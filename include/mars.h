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
#define FRAME_DELAY 30

typedef struct Rocket Rocket;
struct Rocket {
    int x;
    int y;
    int radius;
    int heigth;
    double verticalSpeed;
    double horizontalSpeed;
    double g;
    char* state; // can be LANDED, FLYING, CRASHED
};

typedef struct Edge Edge;
struct Edge {
    int x0;
    int y0;
    int x1;
    int y1;
};

void drawWorld(SDL_Renderer *renderer, Rocket* Rocket, int elapsedTime);
void drawRocket(SDL_Renderer* renderer, Rocket* rocket);
void moveRocket(Rocket* rocket, int elapsedTime);