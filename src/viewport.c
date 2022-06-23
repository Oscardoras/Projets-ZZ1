#include <SDL2/SDL_image.h>

#include <stdlib.h>
#include <time.h>

#include "viewport.h"

#define TEXTURE_FOURMI_NAME "sprites/FourmiGuerriere.png"
#define TEXTURE_BACKGROUND_NAME "sprites/misc/Textures-16.png"
#define TILE_SIZE 16
#define CAMERA_WIDTH 800
#define CAMERA_HEIGHT 600

void initEnvRect(SDL_Rect* environment_rect, int i, int j)
{
    environment_rect->x = TILE_SIZE * i;
    environment_rect->y = TILE_SIZE * j;
    environment_rect->w = TILE_SIZE;
    environment_rect->h = TILE_SIZE;
}
Viewport* init_viewport(int width, int height, Level* level) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error SDL init - %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    Viewport* viewport = malloc(sizeof(Viewport));
    if (viewport) {
        initEnvRect(&viewport->environment_rect[0], 9, 12); // AIR
        initEnvRect(&viewport->environment_rect[1], 2, 3); // DIRT
        initEnvRect(&viewport->environment_rect[2], 1, 1); // PATH
        initEnvRect(&viewport->environment_rect[3], 1, 0); // GRASS
        initEnvRect(&viewport->environment_rect[4], 5, 3); // ROCK
        initEnvRect(&viewport->environment_rect[5], 0, 19); // FRUIT
        initEnvRect(&viewport->environment_rect[6], 11, 18); // LEAVES
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
            
            if (viewport->renderer) {
                viewport->texture_fourmi = IMG_LoadTexture(viewport->renderer,TEXTURE_FOURMI_NAME);
  	            if (viewport->texture_fourmi == NULL) {
		            SDL_Log("Erreur creation texture - %s", SDL_GetError());
                    close_viewport(viewport);
		            exit(EXIT_FAILURE);
	            }
                viewport->texture_background = IMG_LoadTexture(viewport->renderer,TEXTURE_BACKGROUND_NAME);
  	            if (viewport->texture_background == NULL) {
		            SDL_Log("Erreur creation texture - %s", SDL_GetError());
                    close_viewport(viewport);
		            exit(EXIT_FAILURE);
                }
            }
            else {
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
    viewport->camera.x= 0;
    viewport->camera.y= 0;
    viewport->camera.width = CAMERA_WIDTH;
    viewport->camera.height = CAMERA_HEIGHT;
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
        if (viewport->texture_fourmi != NULL) SDL_DestroyTexture(viewport->texture_fourmi);
        if (viewport->texture_background != NULL) SDL_DestroyTexture(viewport->texture_background);
        if (viewport->renderer != NULL) SDL_DestroyRenderer(viewport->renderer);
        if (viewport->window != NULL) SDL_DestroyWindow(viewport->window);
        free(viewport);
        SDL_Quit();
    }
}

void draw_viewport(Viewport* viewport) {
    SDL_SetRenderDrawColor(viewport->renderer, 255, 255, 255, 255);
	SDL_RenderClear(viewport->renderer);
    for(int i = viewport->level->d.min_x; i < viewport->level->d.max_x; ++i)
    {
        for(int j = viewport->level->d.min_y; j < viewport->level->d.max_y; ++j)
        {
            int w, h;
            SDL_GetWindowSize(viewport->window, &w, &h);
            SDL_Rect destination;
            destination.x = i*TILE_SIZE*(w/viewport->camera.width);
            destination.y = j*TILE_SIZE*(h/viewport->camera.height);
            destination.w = TILE_SIZE*(w/viewport->camera.width) + 1;
            destination.h = TILE_SIZE*(h/viewport->camera.height) + 1;
            SDL_RenderCopy(viewport->renderer, viewport->texture_background,
                 &viewport->environment_rect[viewport->level->blocks[(i-viewport->level->d.min_x) + (viewport->level->d.max_x - viewport->level->d.min_x)*(j-viewport->level->d.min_y)]],
                 &destination);
        }
    }
    for(struct ListCell* iterator = viewport->level->entities; iterator; iterator = iterator->next)
    {
        int w, h;
        SDL_GetWindowSize(viewport->window, &w, &h);
        Entity *fourmi = iterator->entity;
        SDL_Rect destination;
        destination.x = fourmi->position.x*TILE_SIZE*w/viewport->camera.width;
        destination.y = fourmi->position.y*TILE_SIZE*h/viewport->camera.height;
        destination.w = viewport->animations[1].rects[time(0)%viewport->animations[1].count].w*w/viewport->camera.width;
        destination.h = viewport->animations[1].rects[time(0)%viewport->animations[1].count].h*h/viewport->camera.height;
        SDL_Point center;
        center.x = destination.w/2;
        center.y = destination.h/2;
		SDL_RenderCopyEx(viewport->renderer, viewport->texture_fourmi,
                 &viewport->animations[1].rects[time(0)%viewport->animations[1].count],
                 &destination, fourmi->position.direction*90, &center, SDL_FLIP_NONE);
    }
    
    SDL_RenderPresent(viewport->renderer);
}
