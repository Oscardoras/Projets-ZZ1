#include <SDL2/SDL.h>

#include "viewport.h"

Viewport* init_viewport(int width, int height, Level* level) {
    if(SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error SDL init - %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    Viewport* viewport = malloc(sizeof(Viewport));
    if(viewport) {
        viewport->width = width;
        viewport->height = height;
        viewport->window = NULL;
        viewport->renderer = NULL;
        
        viewport->camera = malloc(sizeof(Camera));
        if(viewport->camera) {
            viewport->window = SDL_CreateWindow("MarkovAnt",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                width,
                height,
                SDL_WINDOW_RESIZABLE
            );
            
            if(viewport->window) {
                viewport->renderer = SDL_CreateRenderer(viewport->window,
                    -1,
                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                );
                
                if(!viewport->renderer) {
                    SDL_Log("Error SDL Renderer init - %s", SDL_GetError());
                    closeViewport(viewport);
                    viewport = NULL;
                }
            }
            else {
                SDL_Log("Error SDL Window init - %s", SDL_GetError());
                closeViewport(viewport);
                viewport = NULL;
            }
        }
        else {
            SDL_Log("Error Camera alloc");
            free(viewport);
            viewport = NULL;
        } 
    }
    else {
        SDL_Log("Error Viewport alloc");
    }
    
    return viewport;
}