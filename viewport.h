#ifndef VIEWPORT_H
#define VIEWPORT_H
#include "world.h"
#include <SDL2/SDL.h>
typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	World* world;
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
#endif
