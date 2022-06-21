#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <SDL2/SDL.h>

#include "world.h"


typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	World* world;
    unsigned int width;
    unsigned int height;
} Viewport;

/**
 * @brief Creates a new viewport.
 * 
 * @param world the world.
 * @param width the width of the viewport.
 * @param height the height of the viewport.
 */
Viewport* initViewport(World* world, unsigned int width, unsigned int height);

/**
 * @brief Let the user make the starting configuration.
 * 
 * @param viewport the current viewport.
 */
int configInit(Viewport* viewport);

/**
 * @brief The event loop.
 * 
 * @param viewport the wiewport.
 */
void eventLoop(Viewport* viewport);

/**
 * @brief close the viewport.
 *
 * @param viewport if the world has borders.
 */
void closeViewport(Viewport* viewport);

/**
 * @brief draw the cells
 *
 * @param viewport if the world has borders.
 */
void drawCells(Viewport* viewport);


#endif
