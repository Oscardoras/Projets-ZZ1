#include <SDL2/SDL.h>
#include <stdio.h>

#include "viewport.h"
#include "world.h"
#include "world_update.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_COUNT_X 80
#define CELL_COUNT_Y 60
#define BORDER true
#define DEFAULT_DELAY 60


int main() {
    int delay = DEFAULT_DELAY;
    World world = new_world(BORDER, CELL_COUNT_X, CELL_COUNT_Y);
    Viewport* viewport = initViewport(&world, WINDOW_WIDTH, WINDOW_HEIGHT);

    if(!configInit(viewport)) {
        eventLoop(viewport, &delay);
    }  
    closeViewport(viewport);
    free_world(&world);
    
    return EXIT_SUCCESS;
}
