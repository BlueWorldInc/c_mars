#include <mars.h>



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
        (*rocket).x = SCREEN_WIDTH / 2;
        (*rocket).y = SCREEN_HEIGHT / 2;
        (*rocket).radius = 30;
        (*rocket).verticalSpeed = - 2.0;
        (*rocket).heigth = 90;
        (*rocket).g = -3.711;
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
                    run = SDL_FALSE;
                    break;
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
    
    drawRocket(renderer, rocket);
    moveRocket(rocket, elapsedTime);
    SDL_RenderPresent(renderer);
}

void drawRocket(SDL_Renderer* renderer, Rocket* rocket) {
    if (strcmp((*rocket).state, "FLYING") == 0) {
        SDL_Ellipse(renderer, (*rocket).x, (*rocket).y, (*rocket).radius, (*rocket).radius);
        // legs
        SDL_RenderDrawLine(renderer, (*rocket).x - 10, (*rocket).y + 30, (*rocket).x - 30, (*rocket).y + 50);
        SDL_RenderDrawLine(renderer, (*rocket).x + 10, (*rocket).y + 30, (*rocket).x + 30, (*rocket).y + 50);

        SDL_RenderDrawLine(renderer, (*rocket).x - 30, (*rocket).y + 50, (*rocket).x - 30, (*rocket).y + 60);
        SDL_RenderDrawLine(renderer, (*rocket).x + 30, (*rocket).y + 50, (*rocket).x + 30, (*rocket).y + 60);
        
        SDL_RenderDrawLine(renderer, (*rocket).x + 30, (*rocket).y + 60, (*rocket).x + 50, (*rocket).y + 60);
        SDL_RenderDrawLine(renderer, (*rocket).x - 30, (*rocket).y + 60, (*rocket).x - 50, (*rocket).y + 60);
    } else if (strcmp((*rocket).state, "LANDED") == 0) {
        SDL_Ellipse(renderer, (*rocket).x, (*rocket).y, (*rocket).radius, (*rocket).radius);
        // legs
        SDL_RenderDrawLine(renderer, (*rocket).x - 10, (*rocket).y + 30, (*rocket).x - 30, (*rocket).y + 50);
        SDL_RenderDrawLine(renderer, (*rocket).x + 10, (*rocket).y + 30, (*rocket).x + 30, (*rocket).y + 50);

        SDL_RenderDrawLine(renderer, (*rocket).x - 30, (*rocket).y + 50, (*rocket).x - 30, (*rocket).y + 60);
        SDL_RenderDrawLine(renderer, (*rocket).x + 30, (*rocket).y + 50, (*rocket).x + 30, (*rocket).y + 60);
        
        SDL_RenderDrawLine(renderer, (*rocket).x + 30, (*rocket).y + 60, (*rocket).x + 50, (*rocket).y + 60);
        SDL_RenderDrawLine(renderer, (*rocket).x - 30, (*rocket).y + 60, (*rocket).x - 50, (*rocket).y + 60);

        // robot
        SDL_Rect robot_00 = {(*rocket).x - 90, (*rocket).y + 20, 40, 30};
        SDL_RenderDrawRect(renderer, &robot_00);
        SDL_Rect robot_01 = {(*rocket).x - 70, (*rocket).y + 10, 10, 10};
        SDL_RenderDrawRect(renderer, &robot_01);
        SDL_Rect robot_02 = {(*rocket).x - 80, (*rocket).y + 5, 20, 5};
        SDL_RenderDrawRect(renderer, &robot_02);
        SDL_Rect robot_03 = {(*rocket).x - 55, (*rocket).y - 50, 5, 70};
        SDL_RenderDrawRect(renderer, &robot_03);
        SDL_Ellipse(renderer, (*rocket).x - 90, (*rocket).y + 50, 10, 10);
        SDL_Ellipse(renderer, (*rocket).x - 50, (*rocket).y + 50, 10, 10);
    }
}

void moveRocket(Rocket* rocket, int elapsedTime) {

    (*rocket).verticalSpeed += (*rocket).g / 200;

    // negative because y axis is from top to bottom.
    int baseline = (*rocket).y + (*rocket).heigth - (*rocket).radius;
    baseline -= (*rocket).verticalSpeed;
    if (baseline < GROUND_Y) {
        (*rocket).y = baseline - (*rocket).heigth + (*rocket).radius;
    } else {
        if ((*rocket).verticalSpeed > 15) {
            (*rocket).state = "CRASHED";
        } else {
            (*rocket).state = "LANDED";
        }
        (*rocket).y = GROUND_Y - (*rocket).heigth + (*rocket).radius;
    }

}
