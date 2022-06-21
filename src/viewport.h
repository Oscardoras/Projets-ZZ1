#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include <SDL2/SDL.h>

#include "world.h"


typedef struct {
	World* world;
	int delay;
	SDL_Window* window;
	SDL_Renderer* renderer;
    int width;
    int height;
	bool pause;
} Viewport;

/**
 * @brief Creates a new viewport.
 * 
 * @param world a pointer to the world.
 * @param delay the delay.
 * @param width the width of the viewport.
 * @param height the height of the viewport.
 * @return the created viewport.
 */
Viewport* init_viewport(World* world, int delay, int width, int height);

/**
 * @brief closes the viewport.
 *
 * @param viewport the viewport.
 */
void close_viewport(Viewport* viewport);

/**
 * @brief The event loop.
 * 
 * @param viewport the wiewport.
 * @param rule the rule to apply.
 */
void event_loop(Viewport* viewport, Rulestring rule);

/**
 * @brief draws the cells.
 *
 * @param viewport the viewport.
 */
void draw(Viewport* viewport);


#endif
