#include <stdlib.h>

#include "level.h"
#include "viewport.h"
#include "character.h"
#include "matrix.h"
#define WIDTH 800
#define HEIGHT 600
#define WORLD_WIDTH 80
#define WORLD_HEIGHT 60
#define SEED 0
int main(/*int argc, char** argv*/) {

    Level* level = new_level(WORLD_WIDTH, WORLD_HEIGHT, SEED);
    Viewport* viewport = init_viewport(WIDTH, HEIGHT, level);
    close_viewport(viewport);
    free_level(level);
    return EXIT_SUCCESS;

}