#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#include "viewport.h"
#include "world.h"
#include "world_update.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_COUNT_X 80
#define CELL_COUNT_Y 60
#define BORDER false
#define DEFAULT_DELAY 60


int main() {
    int delay = DEFAULT_DELAY;
    bool reset = true;
    int i;

    World world = new_world(BORDER, CELL_COUNT_X, CELL_COUNT_Y);
    Viewport* viewport = initViewport(&world, WINDOW_WIDTH, WINDOW_HEIGHT);

    while(reset) {
        reset = false;
        for(i=0; i<viewport->world->width * viewport->world->height; i++)
            viewport->world->table[i] = false;
        if(!configInit(viewport)) {
            reset = eventLoop(viewport, delay);
        }
    }

    closeViewport(viewport);
    free_world(&world);
    
    return EXIT_SUCCESS;
}
