#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__


#include <SDL2/SDL.h>

#include "viewport.h"
#include "level.h"

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Camera;

typedef struct {
    int width;
    int height;
    Camera* camera;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Level* level;
} Viewport;


/**
 * @brief Creates the viewport and its components
 * 
 * @param width the width of the window.
 * @param height the height of the window.
 * @param level the level to link to the viewport.
 * @return a pointer to the viewport created.
 */
Viewport* initViewport(int width, int height, Level level);

/**
 * @brief Frees the viewport
 * 
 * @param viewport the viewport to free
 */
void closeViewport(Viewport* viewport);

/**
 * @brief Controls the event loop of the game.
 * 
 * @param viewport the viewport of the game.
 */
void eventLoop(Viewport* viewport);

/**
 * @brief Draws the viewport at the end of each event loop.
 * 
 * @param viewport the viewport to draw.
 */
void drawViewport(Viewport* viewport);


#endif