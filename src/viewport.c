#include <SDL2/SDL.h>
#include <stdio.h>

#include "viewport.h"


Viewport* init_viewport(World* world, int delay, int width, int height) {
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
    viewport->delay = delay;
    viewport->width = width;
    viewport->height = height;
    viewport->pause = true;
    
    viewport->window = SDL_CreateWindow(
        "Game of life",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_RESIZABLE
    );
    if (viewport->window == NULL) {
        SDL_Log("Error : SDL window creation - %s", SDL_GetError());
        close_viewport(viewport);
    }
    
    viewport->renderer = SDL_CreateRenderer(
        viewport->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (viewport->renderer == NULL) {
        SDL_Log("Error : SDL renderer creation - %s", SDL_GetError());
        close_viewport(viewport);
    }
    
    return viewport;
}

void close_viewport(Viewport* viewport) {   
    if (viewport != NULL) {
        if (viewport->renderer != NULL) SDL_DestroyRenderer(viewport->renderer);
        if (viewport->window != NULL) SDL_DestroyWindow(viewport->window);
        SDL_Quit();
        free(viewport);
    }
}

void event_loop(Viewport* viewport, Rulestring rule) {
    SDL_Event event;

    bool quit = false;
    bool modified = true;

    int xcell, ycell;
    int icell, jcell;
    bool* cell = NULL;

    FILE* file = NULL;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        viewport->width = event.window.data1;
                        viewport->height = event.window.data2;
                    }
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    if (SDL_GetMouseState(&xcell, &ycell) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                        icell = xcell * viewport->world->width / viewport->width;
                        jcell = ycell * viewport->world->height / viewport->height;                        
                        cell = get_world_cell(viewport->world, icell, jcell);
                        *cell = !(*cell);
                    }
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        case SDLK_LEFT:
                            viewport->delay = (viewport->delay < 200) ? viewport->delay * 1.1 : 200;
                            break;

                        case SDLK_RIGHT:
                            viewport->delay = (viewport->delay > 10) ? viewport->delay / 1.1 : 10;
                            break;
                        
                        case SDLK_p:
                            viewport->pause = !viewport->pause;
                            break;
                        case SDLK_RETURN:
                            viewport->pause = !viewport->pause;
                            break;

                        case SDLK_r:
                        {
                            World tmp = *viewport->world;
                            free_world(viewport->world);
                            *viewport->world = new_world(tmp.borders, tmp.width, tmp.height);
                            viewport->pause = true;
                            modified = true;
                            break;
                        }
                        
                        case SDLK_s:
                            file = fopen("save.txt", "w");
                            if (file) {
                                save_world(viewport->world, file);
                                fclose(file);
                            }
                            break;

                        case SDLK_l:
                            free_world(viewport->world);
                            file = fopen("save.txt", "r");
                            if (file) {
                                *viewport->world = load_world(file);
                                fclose(file);
                            }
                            break;
                        
                        case SDLK_b:
                            viewport->world->borders = !viewport->world->borders;
                            break;
                        
                        case SDLK_KP_MINUS:
                            resize_world(viewport->world, viewport->world->width*2, viewport->world->height*2);
                            break;
                        
                        case SDLK_KP_PLUS:
                            if (viewport->world->width%2 == 0 && viewport->world->height%2 == 0)
                                resize_world(viewport->world, viewport->world->width/2, viewport->world->height/2);
                            break;
                    }
                    break;
            }
        }

        if (!viewport->pause && modified)
            modified = update_world(viewport->world, rule);
        
        draw(viewport);
        SDL_Delay(viewport->delay);
    }
}

void draw(Viewport *viewport) {
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
