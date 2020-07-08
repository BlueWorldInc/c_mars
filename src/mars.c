#include <mars.h>

void SDL_RenderDrawTriangle(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int x2, int y2);
void SDL_RenderFillTriangle(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int x2, int y2);
Edge initEdge(int x0, int y0, int x1, int y1);
void SDL_RenderDrawEdge(SDL_Renderer* renderer, Edge edge);
void DrawSpan(SDL_Renderer* renderer, Span span, int y);
void DrawSpansBetweenEdges(SDL_Renderer* renderer, Edge edge1, Edge edge2);
void drawFlame(SDL_Renderer* renderer, const Rocket* rocket);

int main(int argc, char *argv[])
{

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;
    SDL_Color orange = {255, 127, 40, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    bool init_error = false;
    SDL_bool run = SDL_TRUE;
    srand(time(NULL));
    int actualTime;
    int lastTime;
    int elapsedTime;

    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Cycle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 700, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (NULL == window)
    {
        fprintf(stderr, "Erreur de creation de la fenetre : %s\n", SDL_GetError());
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        init_error = true;
    }
    if (0 != SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        init_error = true;
    }

    if (0 != SDL_RenderClear(renderer))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        init_error = true;
    }

    if (!init_error)
    {

        Rocket* rocket = NULL;
        rocket = malloc(sizeof(*rocket));
        // (*rocket).x = SCREEN_WIDTH / 2;
        (*rocket).x = 50;
        (*rocket).y = SCREEN_HEIGHT / 2;
        (*rocket).y = 30;
        (*rocket).radius = 30;
        (*rocket).verticalSpeed = - 2.0;
        (*rocket).heigth = 90;
        (*rocket).g = -3.711;
        (*rocket).angle = 30;
        (*rocket).thrustPower = 1;
        (*rocket).state = "FLYING";

        actualTime = SDL_GetTicks();
        lastTime = actualTime;
        while (run) {
            SDL_Event event;
            SDL_PollEvent(&event);

            actualTime = SDL_GetTicks();
            elapsedTime = actualTime - lastTime;
            if (elapsedTime > FRAME_DELAY) {
                drawWorld(renderer, rocket, elapsedTime);
                lastTime = actualTime;
            } else {
                SDL_Delay(FRAME_DELAY - elapsedTime);
            }

            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                            (*rocket).thrustPower = 1;
                            break;
                        case SDLK_z:
                            (*rocket).thrustPower = 2;
                            break;
                        case SDLK_e:
                            (*rocket).thrustPower = 3;
                            break;
                        case SDLK_r:
                            (*rocket).thrustPower = 4;
                            break;
                        case SDLK_ESCAPE:
                            run = SDL_FALSE;
                            break;
                        default:
                            run = SDL_FALSE;
                            break;
                    }
            }
        }

        SDL_DestroyWindow(window);
    }

    SDL_Quit();
    return 0;
}

void drawWorld(SDL_Renderer *renderer, Rocket* rocket, int elapsedTime) {
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};

     // clear window
    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

    // draw ground
    SDL_RenderDrawLine(renderer, 0, GROUND_Y, SCREEN_WIDTH, GROUND_Y);

    // texture
    SDL_Texture* texture;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 100, 100);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    // printf("texture: %p\n", texture);
    // SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255);
    
    SDL_SetRenderTarget(renderer, texture);
    drawRocket(renderer, rocket);
    moveRocket(rocket, elapsedTime);
    SDL_Rect rect = {10, 10, 50, 50};
    // SDL_RenderFillRect(renderer, &rect);
    // SDL_RenderDrawRect(renderer, &rect);
    // SDL_RenderDrawLine(renderer, 10, 10, 100, 200);
    // printf("%d ||| %d\n", (*rocket).x, (*rocket).y);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_Rect dst = {900, 600 + (*rocket).y, 500, 500};
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    // printf("w: %d\n", dst.w);
    // printf("h: %d\n", dst.h);
    printf("rocket y: %d\n", (*rocket).y);

    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // transparence
    // SDL_SetRenderDrawColor(renderer, 255, 128, 0, 128);
    // SDL_Rect rect = {500, 750, 100, 100};
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    // SDL_RenderDrawRect(renderer, &rect);

    // drawRocket(renderer, rocket);
    // moveRocket(rocket, elapsedTime);
    SDL_RenderPresent(renderer);
}

void drawRocket(SDL_Renderer* renderer, Rocket* rocket) {
    if (strcmp((*rocket).state, "FLYING") == 0) {
        // replaced (*rocket).y by 30
        drawFlame(renderer, rocket);
        SDL_Ellipse(renderer, (*rocket).x, 30, (*rocket).radius, (*rocket).radius);
        // legs
        SDL_RenderDrawLine(renderer, (*rocket).x - 10, 30 + 30, (*rocket).x - 30, 30 + 50);
        SDL_RenderDrawLine(renderer, (*rocket).x + 10, 30 + 30, (*rocket).x + 30, 30 + 50);

        SDL_RenderDrawLine(renderer, (*rocket).x - 30, 30 + 50, (*rocket).x - 30, 30 + 60);
        SDL_RenderDrawLine(renderer, (*rocket).x + 30, 30 + 50, (*rocket).x + 30, 30 + 60);
        
        SDL_RenderDrawLine(renderer, (*rocket).x + 30, 30 + 60, (*rocket).x + 50, 30 + 60);
        SDL_RenderDrawLine(renderer, (*rocket).x - 30, 30 + 60, (*rocket).x - 50, 30 + 60);

    } else if (strcmp((*rocket).state, "LANDED") == 0) {
        SDL_Ellipse(renderer, (*rocket).x, 30, (*rocket).radius, (*rocket).radius);
        // legs
        SDL_RenderDrawLine(renderer, (*rocket).x - 10, 30 + 30, (*rocket).x - 30, 30 + 50);
        SDL_RenderDrawLine(renderer, (*rocket).x + 10, 30 + 30, (*rocket).x + 30, 30 + 50);

        SDL_RenderDrawLine(renderer, (*rocket).x - 30, 30 + 50, (*rocket).x - 30, 30 + 60);
        SDL_RenderDrawLine(renderer, (*rocket).x + 30, 30 + 50, (*rocket).x + 30, 30 + 60);
        
        SDL_RenderDrawLine(renderer, (*rocket).x + 30, 30 + 60, (*rocket).x + 50, 30 + 60);
        SDL_RenderDrawLine(renderer, (*rocket).x - 30, 30 + 60, (*rocket).x - 50, 30 + 60);

        // robot
        SDL_Rect robot_00 = {(*rocket).x - 90, 30 + 20, 40, 30};
        SDL_RenderDrawRect(renderer, &robot_00);
        SDL_Rect robot_01 = {(*rocket).x - 70, 30 + 10, 10, 10};
        SDL_RenderDrawRect(renderer, &robot_01);
        SDL_Rect robot_02 = {(*rocket).x - 80, 30 + 5, 20, 5};
        SDL_RenderDrawRect(renderer, &robot_02);
        SDL_Rect robot_03 = {(*rocket).x - 55, 30 - 50, 5, 70};
        SDL_RenderDrawRect(renderer, &robot_03);
        SDL_Ellipse(renderer, (*rocket).x - 90, 30 + 50, 10, 10);
        SDL_Ellipse(renderer, (*rocket).x - 50, 30 + 50, 10, 10);
    } else if (strcmp((*rocket).state, "CRASHED") == 0) {
        // Explosion
        SDL_RenderFillTriangle(renderer, (*rocket).x + 10, 30 + 30, (*rocket).x - 30, 30 + 50, (*rocket).x + 30, 30 + 50);
        SDL_RenderFillTriangle(renderer, (*rocket).x + 100, 30 + 30, (*rocket).x + 30, 30 + 50, (*rocket).x - 30, 30 + 50);
        SDL_RenderFillTriangle(renderer, (*rocket).x - 100, 30 + 30, (*rocket).x + 30, 30 + 50, (*rocket).x - 30, 30 + 50);
        SDL_RenderFillTriangle(renderer, (*rocket).x - 130, 30 + 90, (*rocket).x + 30, 30 + 50, (*rocket).x - 30, 30 + 50);
        SDL_RenderFillTriangle(renderer, (*rocket).x + 200, 30 + 90, (*rocket).x + 50, 30 + 50, (*rocket).x - 30, 30 + 50);
        SDL_RenderFillTriangle(renderer, (*rocket).x + 100, 30 + 50, (*rocket).x - 50, 30 + 60, (*rocket).x - 30, 30 + 100);

    }
}

void drawFlame(SDL_Renderer* renderer, const Rocket* rocket) {

    if ((*rocket).thrustPower == 1) {
        SDL_Ellipse(renderer, (*rocket).x, 30 + 40, (*rocket).radius / 4, (*rocket).radius / 3);
    }
    if ((*rocket).thrustPower <= 3) {
        SDL_Ellipse(renderer, (*rocket).x, 30 + 42, (*rocket).radius / 4, (*rocket).radius / 2.5);
    }
    if ((*rocket).thrustPower >= 2) {
        SDL_Ellipse(renderer, (*rocket).x, 30 + 45, (*rocket).radius / 4, (*rocket).radius / 2);
    }
    if ((*rocket).thrustPower >= 3) {
        SDL_Ellipse(renderer, (*rocket).x, 30 + 53, (*rocket).radius / 4, (*rocket).radius / 1.3);
    }
    if ((*rocket).thrustPower >= 4) {
        SDL_Ellipse(renderer, (*rocket).x, 30 + 60, (*rocket).radius / 4, (*rocket).radius / 1);
    }
}

void moveRocket(Rocket* rocket, int elapsedTime) {

    (*rocket).verticalSpeed += (*rocket).g / 200;

    // negative because y axis is from top to bottom.
    int baseline = (*rocket).y + (*rocket).heigth - (*rocket).radius;
    baseline -= (*rocket).verticalSpeed;
    if (baseline < GROUND_Y - 630) {
        (*rocket).y = baseline - (*rocket).heigth + (*rocket).radius;
    } else {
        if ((*rocket).verticalSpeed > 15) {
            (*rocket).state = "CRASHED";
        } else {
            (*rocket).state = "CRASHED";
        }
        (*rocket).y = GROUND_Y - 630 - (*rocket).heigth + (*rocket).radius;
    }

    printf("rocket y: %d ---\n", (*rocket).y);

}


void SDL_RenderDrawTriangle(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int x2, int y2) {

    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x0, y0);

}

// heavily inspired by https://joshbeam.com/articles/triangle_rasterization/
void SDL_RenderFillTriangle(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int x2, int y2) {


    Edge edge0 = initEdge(x0, y0, x1, y1);
    Edge edge1 = initEdge(x1, y1, x2, y2);
    Edge edge2 = initEdge(x2, y2, x0, y0);
    Edge edges[3] = {edge0, edge1, edge2};

    SDL_RenderDrawEdge(renderer, edges[0]);
    SDL_RenderDrawEdge(renderer, edges[1]);
    SDL_RenderDrawEdge(renderer, edges[2]);

    // find tallest edge
    int tallestEdgeIndex = 0;
    int tallestEdgeLength = 0;
    for (int i = 0; i < 3; i++) {
        if ((edges[i].y1 - edges[i].y0) > tallestEdgeLength) {
            tallestEdgeLength = abs(edges[i].y0 - edges[i].y1);
            tallestEdgeIndex = i;
        }
    }

    // short edges
    int shortEdgeIndex_0 = (tallestEdgeIndex + 1) % 3;
    int shortEdgeIndex_1 = (tallestEdgeIndex + 2) % 3;

    DrawSpansBetweenEdges(renderer, edges[tallestEdgeIndex], edges[shortEdgeIndex_0]);
    DrawSpansBetweenEdges(renderer, edges[tallestEdgeIndex], edges[shortEdgeIndex_1]);

}

Edge initEdge(int x0, int y0, int x1, int y1) {
    Edge e;
    if (y0 < y1) {
        e.x0 = x0;
        e.y0 = y0;
        e.x1 = x1;
        e.y1 = y1;
    } else {
        e.x0 = x1;
        e.y0 = y1;
        e.x1 = x0;
        e.y1 = y0;
    }
    return e;
}

Span initSpan(int x0, int x1) {
    Span s;
    if (x0 < x1) {
        s.x0 = x0;
        s.x1 = x1;
    } else {
        s.x0 = x1;
        s.x1 = x0;
    }
    return s;
}

void SDL_RenderDrawEdge(SDL_Renderer* renderer, Edge edge) {
    SDL_RenderDrawLine(renderer, edge.x0, edge.y0, edge.x1, edge.y1);
}

void DrawSpansBetweenEdges(SDL_Renderer* renderer, Edge edge1, Edge edge2) {
    double edge1YDiff = (double) (edge1.y1 - edge1.y0);
    double edge2YDiff = (double) (edge2.y1 - edge2.y0);
    
    if (edge1YDiff == 0 || edge2YDiff == 0) {
        return;
    }

    double edge1XDiff = (double) (edge1.x1 - edge1.x0);
    double edge2XDiff = (double) (edge2.x1 - edge2.x0);
    double factor1 = (double) (edge2.y0 - edge1.y0) / edge1YDiff;
    double factorStep1 = 1.0 / edge1YDiff;
    double factor2 = 0.0;
    double factorStep2 = 1.0 / edge2YDiff;

    for(int i = edge2.y0; i < edge2.y1; i++) {
        Span span = initSpan(edge1.x0 + (int) (edge1XDiff * factor1), edge2.x0 + (int) (edge2XDiff * factor2));
        DrawSpan(renderer, span, i);
        factor1 += factorStep1;
        factor2 += factorStep2;
    }
}

void DrawSpan(SDL_Renderer* renderer, Span span, int y) {
    SDL_RenderDrawLine(renderer, span.x0, y, span.x1, y);
}