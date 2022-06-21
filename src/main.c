#include <stdio.h>
#include <stdbool.h>

#include "viewport.h"
#include "world.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_COUNT_X 80
#define CELL_COUNT_Y 60
#define BORDER true
#define DEFAULT_DELAY 60


int main() {
    World world = new_world(BORDER, CELL_COUNT_X, CELL_COUNT_Y);
    Viewport* viewport = init_viewport(&world, DEFAULT_DELAY, WINDOW_WIDTH, WINDOW_HEIGHT);

    event_loop(viewport, "B3/S23");

    close_viewport(viewport);
    free_world(&world);
    return EXIT_SUCCESS;
}
