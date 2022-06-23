#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include "viewport.h"
#define TEXTURE_NAME "sprites/FourmiGuerriere.png"

Viewport* init_viewport(int width, int height, Level* level) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error SDL init - %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    Viewport* viewport = malloc(sizeof(Viewport));
    if (viewport) {
        viewport->width = width;
        viewport->height = height;
        viewport->level = level;
        viewport->window = NULL;
        viewport->renderer = NULL;
        
        viewport->window = SDL_CreateWindow("MarkovAnt",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_RESIZABLE
        );
        
        if (viewport->window) {
            viewport->renderer = SDL_CreateRenderer(
                viewport->window,
                -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            );
            
            if (!viewport->renderer) {
                SDL_Log("Error SDL Renderer init - %s", SDL_GetError());
                close_viewport(viewport);
                viewport = NULL;
            }
        }
        else {
            SDL_Log("Error SDL Window init - %s", SDL_GetError());
            close_viewport(viewport);
            viewport = NULL;
        }
    } else {
        SDL_Log("Error Viewport alloc");
    }
  	viewport->texture = IMG_LoadTexture(viewport->renderer,TEXTURE_NAME);
  	if (viewport->texture == NULL)
	{
		SDL_Log("Erreur creation texture");
		exit(EXIT_FAILURE);
	} 
    viewport->animations[0].count = 3;
    viewport->animations[1].count = 3;
    viewport->animations[0].rects[0].x = 0;
	viewport->animations[0].rects[0].y = 0;
	viewport->animations[0].rects[0].w = 37;
	viewport->animations[0].rects[0].h = 12;
	viewport->animations[0].rects[1].x = 0;
	viewport->animations[0].rects[1].y = 12*3;
	viewport->animations[0].rects[1].w = 37;
	viewport->animations[0].rects[1].h = 12;
	viewport->animations[0].rects[2].x = 0;
	viewport->animations[0].rects[2].y = 12*4;
	viewport->animations[0].rects[2].w = 37;
	viewport->animations[0].rects[2].h = 12;

    viewport->animations[1].rects[0].x = 0;
	viewport->animations[1].rects[0].y = 12;
	viewport->animations[1].rects[0].w = 37;
	viewport->animations[1].rects[0].h = 12;
	viewport->animations[1].rects[1].x = 0;
	viewport->animations[1].rects[1].y = 12*2;
	viewport->animations[1].rects[1].w = 37;
	viewport->animations[1].rects[1].h = 12;
	viewport->animations[1].rects[2].x = 0;
	viewport->animations[1].rects[2].y = 12*5;
	viewport->animations[1].rects[2].w = 37;
	viewport->animations[1].rects[2].h = 12;
    return viewport;
}

void event_loop(Viewport* viewport) {
    SDL_Event event;
    bool quit = false;
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
            }
        }
        draw_viewport(viewport);
    }
}

void close_viewport(Viewport* viewport) {
    if (viewport != NULL) {
        if (viewport->renderer != NULL) SDL_DestroyRenderer(viewport->renderer);
        if (viewport->window != NULL) SDL_DestroyWindow(viewport->window);
        if (viewport->texture != NULL) SDL_DestroyTexture(viewport->texture);
        free(viewport);
        SDL_Quit();
    }
}

void draw_viewport(Viewport* viewport)
{
    
}
