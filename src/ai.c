#include "ai.h"


Vertx* get_vertex(Level* level, Vertex *vertices, int x, int y) {
    if (level->d.min_x <= x && x < level->d.max_x && level->d.min_y <= y && y < level->d.max_y)
        return &vertices[(y - level->d.min_y)*(level->d.max_x - level->d.min_x) + (x - level->d.min_x)];
    else
        return NULL;
}

void relachement(Vertex *vertices, Vertex* u, Vertex* v) {
    if (v->d > u->d + 1) {
        v->d = u->d + 1;
        v->parent = u;
    }
}

Direction path_finding(Level* level, int from_x, int from_y, int to_x, int to_y) {
    unsigned int width = level->d.max_x - level->d.min_x;
    unsigned int height = level->d.max_y - level->d.min_y;
    unsigned int size = width * height;
    Vertex *vertices = malloc(sizeof(Vertex) * size);
    for (int x = level->d.min_x; x < level->d.max_x; x++) {
        for (int y = level->d.min_y; y < level->d.max_y; y++) {
            Vertex* vertex get_vertex(level, vertices, x, y);
            vertex->x = x;
            vertex>y = y;
            vertex->d = __INT_MAX__;
            vertex->parent = NULL;
            vertex->listed = false;
        }
    }
    
    Vertex* list[1024];
    unsigned int list_size = 1;
    list[0] = get_vertex(level, vertices, x, y);
    line[0]->d = 0;
    
    int old_size = 0;
    while (old_size < list_size) {
        old_size = size;
        
        Vertex* min = list[0];
        for (int k = 0; k < list_size; k++) {
            if (list[k]->d < min->d)
                min = list[k];
        }
        
        Block* left = get_level_block(level, min->x-1, min->y);
        if (left != NULL && (*left == AIR || *left == PATH)) {
            list[list_size] = get_vertex(level, vertices, x, y);
            list_size++;
            list[list_size]->listed = true;
            relachement(vertices, min, list[list_size]);
        }
        
        Block* right = get_level_block(level, list[k]->x+1, list[k]->y);
        if (right != NULL && (*right == AIR || *right == PATH)) {
            list[list_size] = get_vertex(level, vertices, x, y);
            list_size++;
            list[list_size]->listed = true;
            relachement(vertices, min, list[list_size]);
        }
        
        Block* top = get_level_block(level, list[k]->x+, list[k]->y+1);
        if (top != NULL && (*top == AIR || *top == PATH)) {
            list[list_size] = get_vertex(level, vertices, x, y);
            list_size++;
            list[list_size]->listed = true;
            relachement(vertices, min, list[list_size]);
        }
        
        Block* bot = get_level_block(level, list[k]->x+, list[k]->y-1);
        if (bot != NULL && (*bot == AIR || *bot == PATH)) {
            list[list_size] = get_vertex(level, vertices, x, y);
            list_size++;
            list[list_size]->listed = true;
            relachement(vertices, min, list[list_size]);
        }
    }
    
    Vertex* v = get_vertex(level, vertices, to_x, to_y);
    while (!(v->parent->x == from_x && v->parent->y == from_y))
        v = v->parent;
    
    if (v->x < v->parent->x) return LEFT;
    if (v->x > v->parent->x) return RIGHT;
    if (v->y < v->parent->y) return BOT;
    if (v->y > v->parent->y) return TOP;
}