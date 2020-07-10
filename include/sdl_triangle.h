#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

typedef struct Edge Edge;
struct Edge {
    int x0;
    int y0;
    int x1;
    int y1;
};

typedef struct Span Span;
struct Span {
    int x0;
    int x1;
};

void SDL_RenderDrawTriangle(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int x2, int y2);
void SDL_RenderFillTriangle(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int x2, int y2);
Edge initEdge(int x0, int y0, int x1, int y1);
void SDL_RenderDrawEdge(SDL_Renderer* renderer, Edge edge);
void DrawSpan(SDL_Renderer* renderer, Span span, int y);
void DrawSpansBetweenEdges(SDL_Renderer* renderer, Edge edge1, Edge edge2);