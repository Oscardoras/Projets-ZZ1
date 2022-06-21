#include <SDL2/SDL.h>
#include <stdio.h>

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
}

void closeViewport(Viewport *viewport)
{   
    SDL_DestroyRenderer(viewport->renderer);
    SDL_DestroyWindow(viewport->window);
    SDL_Quit();
    free(viewport);
}

int configInit(Viewport* viewport) {
    int initState = 1;
    int quitState = 0;

    SDL_Event event;

    int xcell, ycell;
    int icell, jcell;
    bool* cell = NULL;

    while(initState & !quitState) {
        if(SDL_PollEvent(&event)) {
            switch(event.type) {

                case SDL_QUIT:
                    quitState = 1;
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    if(SDL_GetMouseState(&xcell, &ycell) & SDL_BUTTON(SDL_BUTTON_LEFT))
                        icell = xcell * viewport->world->width / viewport->width;
                        jcell = ycell * viewport->world->height / viewport->height;
                        cell = get_world_cell(viewport->world, icell, jcell);
                        *cell = !(*cell);
                    break;
                
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ENTER:
                            initState = 0;
                            break;
                        
                        case SDLK_s:
                            file = fopen("save.txt");
                            save_world(viewport->world, file);
                            fclose(file);
                            break;

                        case SDLK_l:
                            file = fopen("save.txt");
                            *(viewport->world) = load_world(file);
                            fclose(file);
                            break;
                    }
                    break;

            }
        }
        
        drawCells(viewport);
        SDL_RenderPresent(viewport->renderer);
        SDL_Delay(60);
    }
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

        drawCells(viewport);
        SDL_RenderPresent(viewport->renderer);
        SDL_Delay(60);
        
   }
}

void drawCells(Viewport *viewport)
{
    for(unsigned int x = 0; x < viewport->world->width; ++x)
        for(unsigned int y = 0; y < viewport->world->height; ++y)
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
        for(unsigned int x = 0; x < viewport->world->width; ++x)
                SDL_RenderDrawLine(viewport->renderer, x*(viewport->width/viewport->world->width), 0, x*(viewport->width/viewport->world->width), viewport->height);
        for(unsigned int y = 0; y < viewport->world->width; ++y)
                SDL_RenderDrawLine(viewport->renderer, 0, y*(viewport->height/viewport->world->height), viewport->width, y*(viewport->height/viewport->world->height));

    }
    SDL_RenderPresent(viewport->renderer);
}
