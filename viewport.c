#include <SDL2/SDL.h>
#include <stdio.h>

#include "viewport.h"
#include "world_update.h"


Viewport* initViewport(World* world, int width, int height) {
    Viewport* viewport = malloc(sizeof(Viewport));
    if (viewport == NULL) {
        SDL_Log("Error : viewport creation");
        exit(EXIT_FAILURE);
    }
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    viewport->world = world;
    viewport->width = width;
    viewport->height = height;
    
    viewport->window = SDL_CreateWindow(
        "Game of life",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_RESIZABLE
    );
    if (viewport->window == NULL) {
        SDL_Log("Error : SDL window creation - %s", SDL_GetError());
        closeViewport(viewport);
    }
    
    viewport->renderer = SDL_CreateRenderer(
        viewport->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (viewport->renderer == NULL) {
        SDL_Log("Error : SDL renderer creation - %s", SDL_GetError());
        closeViewport(viewport);
    }
    
    return viewport;
}

void closeViewport(Viewport* viewport) {   
    if (viewport != NULL) {
        if (viewport->renderer != NULL) SDL_DestroyRenderer(viewport->renderer);
        if (viewport->window != NULL) SDL_DestroyWindow(viewport->window);
        SDL_Quit();
        free(viewport);
    }
}

int configInit(Viewport* viewport) {
    int initState = 1;
    int quitState = 0;

    SDL_Event event;
    FILE * file = NULL;

    int xcell, ycell;
    int icell, jcell;
    bool* cell = NULL;

    while(initState & !quitState) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {

                case SDL_QUIT:
                    quitState = 1;
                    break;
                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        viewport->width = event.window.data1;
                        viewport->height = event.window.data2;
                        drawCells(viewport);
                        }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(SDL_GetMouseState(&xcell, &ycell) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                        icell = xcell * viewport->world->width / viewport->width;
                        jcell = ycell * viewport->world->height / viewport->height;                        
                        cell = get_world_cell(viewport->world, icell, jcell);
                        *cell = !(*cell);
                    }
                    break;
                
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_RETURN:
                            initState = 0;
                            break;
                        
                        case SDLK_s:
                            file = fopen("save.txt", "w");
                            save_world(viewport->world, file);
                            fclose(file);
                            break;

                        case SDLK_l:
                            file = fopen("save.txt", "r");
                            *(viewport->world) = load_world(file);
                            fclose(file);
                            break;
                    }
                    break;

            }
        }
        
        drawCells(viewport);
        SDL_Delay(60);
    }
    return quitState;
}

void eventLoop(Viewport* viewport, int delay) {
    SDL_Event event;
    bool running = true;
    bool modified = true;
    while (running) {
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        viewport->width = event.window.data1;
                        viewport->height = event.window.data2;
                        drawCells(viewport);
                        }
                break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_RETURN:
                            running = false;
                            break;
                        case SDLK_LEFT:
                            delay = (delay > 10) ? delay-1 : 10;
                            break;
                        case SDLK_RIGHT:
                            delay = (delay < 500) ? delay+1 : 500;
                            break;
                    }
                    break;
            }
        }

        if (modified)
            modified = update_world(viewport->world, "B3/S23");
        
        drawCells(viewport);
        SDL_Delay(delay);
    }
}

void drawCells(Viewport *viewport) {
    for(int x = 0; x < viewport->world->width; ++x)
        for(int y = 0; y < viewport->world->height; ++y)
        {
            if(!(*get_world_cell(viewport->world, x, y)))
                SDL_SetRenderDrawColor(viewport->renderer, 255, 255, 255, 255);
            else
                SDL_SetRenderDrawColor(viewport->renderer, 220, 220, 220, 255);

            SDL_Rect rect;
            rect.w = (float)viewport->width/(float)viewport->world->width+1;
            rect.h = (float)viewport->height/(float)viewport->world->height+1;
            rect.x = x*(float)viewport->width/(float)viewport->world->width;
            rect.y = y*(float)viewport->height/(float)viewport->world->height;
            SDL_RenderFillRect(viewport->renderer, &rect);
        }
    SDL_SetRenderDrawColor(viewport->renderer, 100, 100, 100, 255);
    for(int x = 1; x < viewport->world->width+1; ++x)
        SDL_RenderDrawLine(viewport->renderer, x*((float)viewport->width/(float)viewport->world->width), 0, x*((float)viewport->width/(float)viewport->world->width), viewport->height);
    for(int y = 1; y < viewport->world->height+1; ++y)
        SDL_RenderDrawLine(viewport->renderer, 0, y*((float)viewport->height/(float)viewport->world->height), viewport->width, y*((float)viewport->height/(float)viewport->world->height));

    SDL_RenderPresent(viewport->renderer);
}
