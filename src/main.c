#include <stdlib.h>

#include "level.h"
#include "viewport.h"
#include "character.h"
#include "matrix.h"


int main(int argc, char** argv) {
    Level level;
    Viewport* viewport = initViewport(50, 60, level);
    return EXIT_SUCCESS;
}