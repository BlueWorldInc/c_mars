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


        while (run) {
            SDL_Event event;
            SDL_PollEvent(&event);

            // drawWorld(renderer, rocket);

            SDL_Delay(10);
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

void drawWorld(SDL_Renderer *renderer, Cycle* cycle) {
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};

     // clear window
    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

    // draw ground
    SDL_RenderDrawLine(renderer, 0, GROUND_Y, SCREEN_WIDTH, GROUND_Y);
    
    drawCycle(renderer, cycle);
    drawRock(renderer, cycle);
    SDL_RenderPresent(renderer);
}

void drawRocket(SDL_Renderer* renderer, Rocket* rocket) {

}
