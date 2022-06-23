#include "ai.h"


Direction path_finding(Level* level, int from_x, int from_y, int to_x, int to_y) {
    unsigned int width = level->d.max_x - level->d.min_x;
    unsigned int height = level->d.max_y - level->d.min_y;
    unsigned int size = width * height;
    Vertex *vertices = malloc(sizeof(Vertex) * size);
    for (unsigned int k = 0; k < size; k++) {
        vertices->d = __INT_MAX__;
        vertices->parent = NULL;
    }
    vertices[from_y*width + from_x].d = 0;
    
    
}