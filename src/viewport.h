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
 * @brief Creates a viewport and its components.
 * 
 * @param width the width of the window.
 * @param height the height of the window.
 * @param level the level displayed in the window.
 * @return the created viewport.
 */
Viewport* init_viewport(int width, int height, Level* level);

/**
 * @brief Frees a viewport.
 * 
 * @param viewport the viewport to free.
 */
void close_viewport(Viewport* viewport);

/**
 * @brief The display loop.
 * 
 * @param viewport the viewport to loop.
 */
void event_loop(Viewport* viewport);

/**
 * @brief Draws a viewport.
 * 
 * @param viewport the viewport to draw.
 */
void draw_viewport(Viewport* viewport);


#endif