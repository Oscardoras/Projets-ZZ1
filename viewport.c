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
    viewport->window = SDL_CreateWindow("Game of life",
                                                  SDL_WINDOWPOS_CENTERED,
                                                  SDL_WINDOWPOS_CENTERED, width,
                                                  height,
                                                  SDL_WINDOW_OPENGL);
    viewport->world = world;
    viewport->renderer = SDL_CreateRenderer(viewport->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    viewport->width = width;
    viewport->height = height;
    
    return viewport;
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
                case SDL_WINDOWEVENT_RESIZED:
                    viewport->width = event.window.data1;
                    viewport->height = event.window.data2;
            }
        }
        SDL_RenderPresent(viewport->renderer);
        SDL_Delay(60);
        drawCells(viewport);
   }
}

void drawCells(Viewport *viewport)
{
    for(int x = 0; x < viewport->world->width; ++x)
        for(int y = 0; y < viewport->world->height; ++y)
        {
            if(get_world_cell(viewport->world, x, y))
                SDL_SetRenderDrawColor(viewport->renderer, 255, 255, 255, 255);
            else
                SDL_SetRenderDrawColor(viewport->renderer, 220, 220, 220, 255);

            SDL_Rect rect;
            rect.w = viewport->width/viewport->world->width;
            rect.h = viewport->height/viewport->world->height;
            rect.x = x*rect.w;
            rect.y = y*rect.h;
            SDL_RenderFillRect(viewport->renderer, &rect);
        }
    if(viewport->world->borders)
    {
        SDL_SetRenderDrawColor(viewport->renderer, 100, 100, 100, 255);
        for(int x = 0; x < viewport->world->width; ++x)
                SDL_RenderDrawLine(viewport->renderer, x*(viewport->width/viewport->world->width), 0, x*(viewport->width/viewport->world->width), viewport->height);
        for(int y = 0; y < viewport->world->width; ++y)
                SDL_RenderDrawLine(viewport->renderer, 0, y*(viewport->height/viewport->world->height), viewport->width, y*(viewport->height/viewport->world->height));

    }
    SDL_RenderPresent(viewport->renderer);
}
