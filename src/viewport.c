#include <SDL2/SDL_image.h>

#include <stdlib.h>
#include <time.h>

#include "gameplay.h"
#include "viewport.h"

#define TILE_SIZE 16


void init_block_rect(Viewport* viewport, unsigned int id, int i, int j) {
    viewport->animations.blocks[id].tileset = viewport->tilesets.blocks;
    viewport->animations.blocks[id].rectangles[0].x = i;
    viewport->animations.blocks[id].rectangles[0].y = j;
    viewport->animations.blocks[id].rectangles[0].w = TILE_SIZE;
    viewport->animations.blocks[id].rectangles[0].h = TILE_SIZE;
    viewport->animations.blocks[id].frames = 1;
}

void init_entity_rect(Viewport* viewport, EntityTypeName type, State state, int frames, int i, int j, int w, int h) {
    if (state == 1)
        viewport->animations.entities[type] = malloc(sizeof(Animation) * (entity_types[type]->markov->size -1));

    Animation* animation = &viewport->animations.entities[type][state-1];
    animation->tileset = viewport->tilesets.entities;
    animation->frames = frames;
    for (int k = 0; k < frames; k++) {
        animation->rectangles[k].x = i;
        animation->rectangles[k].y = j + k*h;
        animation->rectangles[k].w = w;
        animation->rectangles[k].h = h;
    }
}

Viewport* create_viewport(int width, int height, Level* level) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_WINDOW_MAXIMIZED)) {
        SDL_Log("Error SDL - %s", SDL_GetError());
        return NULL;
    }
    
    Viewport* viewport = malloc(sizeof(Viewport));
    if (viewport == NULL) {
        SDL_Quit();
        return NULL;
    }

    viewport->width = width;
    viewport->height = height;
    viewport->level = level;
    viewport->camera.x = viewport->level->d.min_x;
    viewport->camera.y = viewport->level->d.max_y-1;
    viewport->camera.zoom = TILE_SIZE;
    viewport->window = NULL;
    viewport->renderer = NULL;
    viewport->tilesets.blocks = NULL;
    viewport->tilesets.entities = NULL;
    
    viewport->window = SDL_CreateWindow("MarkovAnt",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_RESIZABLE
    );
    if (viewport->window == NULL) {
        SDL_Log("Error SDL - %s", SDL_GetError());
        close_viewport(viewport);
        return NULL;
    }
    
    viewport->renderer = SDL_CreateRenderer(
        viewport->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (viewport->renderer == NULL) {
        SDL_Log("Error SDL - %s", SDL_GetError());
        close_viewport(viewport);
        return NULL;
    }
    
    viewport->tilesets.blocks = IMG_LoadTexture(viewport->renderer, "textures/blocks.png");
    if (viewport->tilesets.blocks == NULL) {
        SDL_Log("Error IMG");
        close_viewport(viewport);
        return NULL;
    }
    viewport->tilesets.entities = IMG_LoadTexture(viewport->renderer, "textures/entities.png");
    if (viewport->tilesets.entities == NULL) {
        SDL_Log("Error IMG");
        close_viewport(viewport);
        return NULL;
    }

    init_block_rect(viewport, BLOCK_AIR, 144, 192);
    init_block_rect(viewport, BLOCK_DIRT, 32, 17);
    init_block_rect(viewport, BLOCK_PATH, 17, 17);
    init_block_rect(viewport, BLOCK_GRASS, 17, 0);
    init_block_rect(viewport, BLOCK_ROCK, 160, 17);
    init_block_rect(viewport, BLOCK_LEAVES, 11, 18);

    init_entity_rect(viewport, ENTITY_WORKER, WORKER_WAITING, 1, 56, 0, 32, 12);
    init_entity_rect(viewport, ENTITY_WORKER, WORKER_DEAD, 1, 56, 12, 32, 12);
    init_entity_rect(viewport, ENTITY_WORKER, WORKER_SEEKING_FOOD_OUT, 2, 56, 24, 32, 12);
    init_entity_rect(viewport, ENTITY_WORKER, WORKER_SEEKING_FOOD_IN, 2, 56, 24, 32, 12);
    init_entity_rect(viewport, ENTITY_WORKER, WORKER_TAKING_FOOD, 2, 55, 48, 32, 12);
    init_entity_rect(viewport, ENTITY_WORKER, WORKER_BUILDING, 2, 56, 24, 32, 12);

    init_entity_rect(viewport, ENTITY_SOLDIER, SOLDIER_WAITING, 1, 0, 0, 55, 16);

    init_entity_rect(viewport, ENTITY_QUEEN, QUEEN_WAITING, 1, 94, 0, 72, 24);
    init_entity_rect(viewport, ENTITY_QUEEN, QUEEN_WALKING, 2, 94, 48, 72, 24);
    init_entity_rect(viewport, ENTITY_QUEEN, QUEEN_READY_LAY, 1, 94, 0, 72, 24);
    init_entity_rect(viewport, ENTITY_QUEEN, QUENN_LAYING, 1, 94, 0, 72, 24);

    init_entity_rect(viewport, ENTITY_MANTIS, SOLDIER_WAITING, 1, 166, 0, 91, 37);

    init_entity_rect(viewport, ENTITY_PHEROMONE, 1, 1, 282, 208, 16, 16);

    init_entity_rect(viewport, ENTITY_FOOD, 1, 1, 266, 0, 32, 32);

    return viewport;
}

void close_viewport(Viewport* viewport) {
    if (viewport != NULL) {
        if (viewport->tilesets.entities != NULL) {
            SDL_DestroyTexture(viewport->tilesets.entities);
            for (int k = 0; k < ENTITY_TYPES; k++)
                free(viewport->animations.entities[k]);
        }
        if (viewport->tilesets.blocks != NULL) SDL_DestroyTexture(viewport->tilesets.blocks);

        if (viewport->renderer != NULL) SDL_DestroyRenderer(viewport->renderer);
        if (viewport->window != NULL) SDL_DestroyWindow(viewport->window);
        free(viewport);
        SDL_Quit();
    }
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

                        if (viewport->camera.x + viewport->width/viewport->camera.zoom > viewport->level->d.max_x-1)
                            viewport->camera.zoom = viewport->width / (viewport->level->d.max_x-1 - viewport->camera.x);
                        
                        if (viewport->camera.y - viewport->height/viewport->camera.zoom < viewport->level->d.min_y)
                            viewport->camera.zoom = viewport->height / (viewport->camera.y - viewport->level->d.min_y);
                    }
                    break;
                case SDL_KEYDOWN: {
                    switch(event.key.keysym.sym) {
                        case SDLK_LEFT:
                            if (viewport->camera.x > viewport->level->d.min_x)
                                viewport->camera.x--;
                        break;
                        case SDLK_RIGHT:
                            if (viewport->camera.x + viewport->width/viewport->camera.zoom < viewport->level->d.max_x-1)
                                viewport->camera.x++;
                        break;
                        case SDLK_UP:
                            if (viewport->camera.y < viewport->level->d.max_y-1)
                                viewport->camera.y++;
                        break;
                        case SDLK_DOWN:
                            if (viewport->camera.y - viewport->height/viewport->camera.zoom > viewport->level->d.min_y)
                                viewport->camera.y--;
                        break;
                    }
                    break;
                }
                /*
                case SDL_MOUSEBUTTONDOWN: {
                    int w, h;
                    SDL_GetWindowSize(viewport->window, &w, &h);
                    int printable_x = 0;
                    int printable_y = 0;

                    for(int i = viewport->level->d.min_x; i < viewport->level->d.max_x; ++i)
                        if(i * TILE_SIZE >= (viewport->camera.x * TILE_SIZE) && i * TILE_SIZE <= (viewport->camera.x*TILE_SIZE + viewport->camera.width))
                                ++printable_x;
                    for(int j = viewport->level->d.min_y; j < viewport->level->d.max_y; ++j)
                        if(j * TILE_SIZE >= (viewport->camera.y * TILE_SIZE) && j * TILE_SIZE <= (viewport->camera.y*TILE_SIZE + viewport->camera.height))
                                ++printable_y;
                    printable_x*=TILE_SIZE;
                    printable_y*=TILE_SIZE;
                    int x;
                    int y;
                    SDL_GetMouseState(&x, &y);
                    unsigned int targetX = viewport->camera.x+x/(w/((printable_x/TILE_SIZE)));
                    unsigned int targetY = viewport->camera.y+(y)/(h/((printable_y/TILE_SIZE)));
                    targetY = (viewport->level->d.max_y)  - targetY + viewport->level->d.min_y;
                    
                    if (SDL_BUTTON(SDL_BUTTON_LEFT)) 
                        add_pheromone(viewport->level, targetX, targetY, (PheromoneType)PHEROMONE_DIG);
                    else if(SDL_BUTTON(SDL_BUTTON_RIGHT))
                        add_pheromone(viewport->level, targetX, targetY, (PheromoneType)PHEROMONE_FILL);
                    break;
                }
                case SDL_MOUSEWHEEL: {
                    if(event.wheel.y > 0 && viewport->camera.width/TILE_SIZE < viewport->level->d.max_x - viewport->level->d.min_x) // scroll up
                    {
                        viewport->camera.width *= 1.1;
                        viewport->camera.height *= 1.1;
                    }
                    else if(event.wheel.y < 0 && viewport->camera.width > 0 && viewport->camera.height > 0) // scroll down
                    {
                        viewport->camera.width /= 1.1;
                        viewport->camera.height /= 1.1;
                    }
                    if(viewport->camera.x+1 > viewport->level->d.max_x-(viewport->camera.width/TILE_SIZE))
                    {
                        if(viewport->camera.width/TILE_SIZE > viewport->level->d.max_x - viewport->level->d.min_x)
                            viewport->camera.x = viewport->level->d.min_x;
                        else
                            viewport->camera.x = viewport->level->d.max_x-(viewport->camera.width/TILE_SIZE);
                    }
                    if(viewport->camera.y+1 > viewport->level->d.max_y-(viewport->camera.height/TILE_SIZE))
                    {
                        if(viewport->camera.height/TILE_SIZE > viewport->level->d.max_y - viewport->level->d.min_y)
                            viewport->camera.y = viewport->level->d.min_y;
                        else
                            viewport->camera.y = viewport->level->d.max_y-(viewport->camera.height/TILE_SIZE);
                    }
                    break;
                }
                */
            }
        }
        draw_viewport(viewport);
        viewport->animation_loop++;

        if (viewport->animation_loop >= 10) {
            viewport->animation_loop = 0;
            game_loop_iteration(viewport->level);
        }
        SDL_Delay(10);
    }
}

void copy_texture(Viewport* viewport, Animation* animation, SDL_Rect* rect, Direction direction) {
    SDL_RenderCopyEx(viewport->renderer,
        animation->tileset,
        &animation->rectangles[viewport->animation_loop % animation->frames],
        rect,
        direction == DIRECTION_TOP ? 90. : (direction == DIRECTION_BOT ? -90. : 0.),
        NULL,
        direction == DIRECTION_RIGHT ?  SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
    );
}

void draw_viewport(Viewport* viewport) {
    SDL_SetRenderDrawColor(viewport->renderer, 0, 0, 0, 255);
	SDL_RenderClear(viewport->renderer);

    //Draw level.
    for (int x = viewport->level->d.min_x; x < viewport->level->d.max_x; x++) {
        for (int y = viewport->level->d.min_y; y < viewport->level->d.max_y; y++) {
            SDL_Rect destination;
            destination.x = (int) ((x - viewport->camera.x) * viewport->camera.zoom);
            destination.y = (int) (-(y - viewport->camera.y) * viewport->camera.zoom);
            destination.w = viewport->camera.zoom;
            destination.h = viewport->camera.zoom;
            copy_texture(viewport, &viewport->animations.blocks[*get_level_block(viewport->level, x, y)], &destination, 0);
        }
    }

    //Draw entities.
    for (struct EntityListCell* it = viewport->level->entities; it != NULL; it = it->next)
        if (it->entity->state > 0) {
            Animation* animation = &viewport->animations.entities[it->entity->type][it->entity->state-1];
            SDL_Rect destination;
            destination.x = (int) ((it->entity->location.x - viewport->camera.x) * viewport->camera.zoom);
            destination.y = (int) (-(it->entity->location.y - viewport->camera.y) * viewport->camera.zoom);
            destination.w = viewport->camera.zoom * animation->rectangles->w/TILE_SIZE;
            destination.h = viewport->camera.zoom * animation->rectangles->h/TILE_SIZE;
            copy_texture(viewport, animation, &destination, it->entity->location.direction);
        }
    
    SDL_RenderPresent(viewport->renderer);
}
