#include <stdio.h>
#include <stdlib.h>

#include "level.h"

Level* new_level(int min_x, int max_x, int min_y, int max_y, int seed) {
    Level* level = malloc(sizeof(Level));
    if (level != NULL) {
        level->blocks = malloc(sizeof(Block) * (level->max_x - level->min_x) * (level->max_y - level->min_y));
        if (level->blocks != NULL) {
            level->blocks[0] = Air;
            level->min_x = min_x;
            level->max_x = max_x;
            level->min_y = min_y;
            level->max_y = max_y;
            level->seed = seed;
            level->entities = NULL;
            
            //TODO : generate the world
        } 
        else free(level);
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
    fprintf(file, "%d %d %d %d %d\n", level->min_x, level->max_x, level->min_y, level->max_y, level->seed);
    
    int size = (level->max_x - level->min_x) * (level->max_y - level->min_y);
    for (int k = 0; k < size; k++)
        fprintf("%d\n", (int) level->blocks[k]);
    
    struct ListCell* cell = level->entities;
    while (cell != NULL) {
        save_entity(cell->entity, file);
        cell = cell->next;
    }
}

Level* load_level(FILE* file) {
    Level* level = malloc(sizeof(Level));
    
    if (level != NULL) {
        char buffer[256];
        fgets(buffer, 256, file);
        char* b = buffer;
        char value[256];
        char *v;
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->min_x = atoi(v);
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->max_x = atoi(v);
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->min_y = atoi(v);
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->max_y = atoi(v);
        
        for (v = value; *b != '\n'; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->seed = atoi(v);
        
        int size = (level->max_x - level->min_x) * (level->max_y - level->min_y);
        for (int i = 0; i < size; i++) {
            fgets(buffer, 256, file);
            b = buffer;
            for (v = value; *b != '\n'; b++) {
                *v = *b;
                v++;
            }
            *v = '\0';
            level->blocks[i] = atoi(v);
        }
        
        Entity* entity = load_entity(file);
        while (entity != NULL && add_level_entity(level, entity))
            entity = load_entity(file);
    }
    
    return level;
}

bool resize_level(Level* level, int min_x, int max_x, int min_y, int max_y) {
    Level* level2 = new_level(min_x, max_x, min_y, max_y, 0);
    if (level2 != NULL) {
        for (int x = level->min_x; x < level->max_x; x++) {
            for (int y = level->min_y; y < level->max_y; y++) {
                Block* b = get_level_block(level, x, y);
                Block* b2 = get_level_block(level2, x, y);
                if (b != NULL)
                    *b2 = *b;
                //TODO : generate the world
            }
        }
            
        return true;
    } return false;
}

Block* get_level_block(Level* level, int x, int y) {
    if (level->min_x <= x && x < level->max_x)
        return &level->blocks[(y - level->min_y)*(level->max_x - level->min_x) + (x - level->min_x)];
    else
        return NULL;
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