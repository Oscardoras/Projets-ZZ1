#include "viewport.h"
Viewport *initViewport(World *world, unsigned int width, unsigned int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
    SDL_Log("Erreur initialisation SDL");
    exit(EXIT_FAILURE);
    }
    Viewport* viewport = (Viewport*)malloc(sizeof(Viewport));
    if(!viewport)
    {
        SDL_Log("Erreur creation viewport");
        exit(EXIT_FAILURE);
    }
    viewport->window = window = SDL_CreateWindow("Game of life",
                                                  SDL_WINDOWPOS_CENTERED,
                                                  SDL_WINDOWPOS_CENTERED, width,
                                                  height,
                                                  SDL_WINDOW_OPENGL);
    viewport->world = world;
    viewport->renderer = SDL_CreateRenderer(viewport->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void closeViewport(Viewport *viewport)
{
    SDL_DestroyWindow(viewport->window);
    SDL_Quit();
    free(viewport);
}

void eventLoop(Viewport *viewport)
{
    SDL_Event event;
    bool continuer = true;
    while (continuer) {
        if(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    continuer = false;
                break;
            }
        }
        SDL_RenderPresent(viewport->renderer);
        SDL_Delay(60);
   }
}
