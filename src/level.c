#include <sdtio.h>
#include <stdlib.h>

#include "level.h"
#include <stdlib.h>

Level* new_level(int min_x, int max_x, int min_y, int max_y, int seed) {
    Level* level = malloc(sizeof(Level));
    if (level != NULL) {
        level->blocks = malloc(sizeof(Block) * (level->max_x - level->min_x) * (level->max_y - level->min_y));
        if (level->blocks != NULL) {
            level->min_x = min_x;
            level->max_x = max_x;
            level->min_y = min_y;
            level->max_y = max_y;
            level->seed = seed;
            level->entities = NULL;
            
            //TODO : generate the world
        } else free(level);
    }
    
    return level;
}

void free_level(Level* level) {
    if (level != NULL) {
        if (level->blocks != NULL)
            free(level->blocks);
        
        struct ListCell* cell = level->entities;
        while (cell != NULL) {
            struct ListCell* tmp = cell;
            cell = cell->next;
            free(tmp);
        }
        
        free(level);
    }
}

void save_level(Level* level, FILE* file) {
    fprintf(file, "%d %d %d %d %d\n", level->min_x, level->max_x, level->min_y, level->max_y);
    
    unsigned int size = (level->max_x - level->min_x) * (level->max_y - level->min_y);
    for (int k = 0; k < size; k++)
        fprintf("%d\n", level->block[k]);
    
    struct ListCell* cell = level->entities;
    while (cell != NULL) {
        save_entity(cell->entity, file);
        cell = cell->next;
    }
}

Level* load_level(FILE* file) {
    char buffer[256];
    //fgets(buffer, 256, );
}

bool resize_level(Level* level, int min_x, int max_x, int min_y, int max_y) {
    Level* level2 = new_level(min_x, max_x, min_y, max_y, 0);
    if (new_blocks != NULL) {
        for (int x = level->min_x; x < level->max_x; x++) {
            for (int y = level->min_y; y < level->max_y; y++) {
                Block* b = get_level_block(level, x, y);
                Block* b2 = get_level_block(level2, x, y);
                if (b != NULL)
                    *b2 = *b
                //TODO : generate the world
            }
        }
            
        return true;
    } return false;
}

Block* get_level_block(Level* level, int x, int y) {
    if (level->min_x <= x && x < level->max_x)
    return &level->blocks[(y - level->min_y)*(level->max_x - level->min_x) + (x - level->min_x)];
}

bool add_level_entity(Level* level, Entity* entity) {
    struct ListCell* cell = malloc(sizeof(struct ListCell));
    if (cell != NULL) {
        cell->entity = entity;
        cell->next = level->entities;
        return true;
    } else return false;
}

void clean_level_entities(Level* level) {
    struct ListCell** ptr = &level->entities;
    while (*ptr != NULL) {
        if ((*ptr)->entity->current_state == Void) {
            struct ListCell* tmp = *ptr;
            *ptr = (*ptr)->next;
            free(tmp);
        } else ptr = &(*ptr)->next;
    }
}