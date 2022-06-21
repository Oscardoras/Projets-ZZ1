#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

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
 * @param world a pointer to the world.
 * @param width the width of the viewport.
 * @param height the height of the viewport.
 * @return the created viewport.
 */
Viewport* initViewport(World* world, int width, int height);

/**
 * @brief closes the viewport.
 *
 * @param viewport the viewport.
 */
void closeViewport(Viewport* viewport);

/**
 * @brief Lets the user make the starting configuration.
 * 
 * @param viewport the current viewport.
 */
int configInit(Viewport* viewport);

/**
 * @brief The event loop.
 * 
 * @param viewport the wiewport.
 * @param delay the delay.
 * @return if the simulation will be resetted or not
 */
bool eventLoop(Viewport* viewport, int delay);

/**
 * @brief draws the cells.
 *
 * @param viewport the viewport.
 */
void drawCells(Viewport* viewport);


#endif
