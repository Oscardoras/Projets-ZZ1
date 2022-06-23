#include <stdlib.h>

#include "level.h"


Level* new_level(int min_x, int max_x, int min_y, int max_y, int seed) {
    Level* level = malloc(sizeof(Level));
    if (level != NULL) {
        int size = (max_x - min_x) * (max_y - min_y);
        level->blocks = malloc(sizeof(Block) * size);
        if (level->blocks != NULL) {
            level->min_x = min_x;
            level->max_x = max_x;
            level->min_y = min_y;
            level->max_y = max_y;
            level->seed = seed;
            level->entities = NULL;
            
            //TODO : generate the world
        } else {
            free(level);
            level = NULL;
        }
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
        fprintf(file, "%d\n", (int) level->blocks[k]);
    
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
        char* v;
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->min_x = atoi(value);
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->max_x = atoi(value);
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->min_y = atoi(value);
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->max_y = atoi(value);
        
        for (v = value; *b != '\n'; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->seed = atoi(value);
        
        int size = (level->max_x - level->min_x) * (level->max_y - level->min_y);
        level->blocks = malloc(sizeof(Level) * size);
        if (level->blocks != NULL) {
            for (int i = 0; i < size; i++) {
                fgets(buffer, 256, file);
                b = buffer;
                for (v = value; *b != '\n'; b++) {
                    *v = *b;
                    v++;
                }
                *v = '\0';
                level->blocks[i] = atoi(value);
            }
        
            Entity* entity = load_entity(file);
            while (entity != NULL && add_level_entity(level, entity))
                entity = load_entity(file);
        } else {
            free(level);
            level = NULL;
        }
    }
    
    return level;
}

bool resize_level(Level* level, int min_x, int max_x, int min_y, int max_y) {
    Level* level2 = new_level(min_x, max_x, min_y, max_y, 0);
    if (level2 != NULL) {
        for (int x = level2->min_x; x < level2->max_x; x++) {
            for (int y = level2->min_y; y < level2->max_y; y++) {
                Block* b = get_level_block(level, x, y);
                Block* b2 = get_level_block(level2, x, y);
                if (b != NULL)
                    *b2 = *b;
                //TODO : generate the world
            }
        }
        
        free(level->blocks);
        *level = *level2;
        return true;
    } return false;
}

Block* get_level_block(Level* level, int x, int y) {
    if (level->min_x <= x && x < level->max_x && level->min_y <= y && y < level->max_y)
        return &level->blocks[(y - level->min_y)*(level->max_x - level->min_x) + (x - level->min_x)];
    else
        return NULL;
}

bool add_level_entity(Level* level, Entity* entity) {
    struct ListCell* cell = malloc(sizeof(struct ListCell));
    if (cell != NULL) {
        cell->entity = entity;
        cell->next = level->entities;
        level->entities = cell;
        return true;
    } else return false;
}

void clean_level_entities(Level* level) {
    struct ListCell** ptr = &level->entities;
    while (*ptr != NULL) {
        if ((*ptr)->entity->state == VOID) {
            struct ListCell* tmp = *ptr;
            *ptr = (*ptr)->next;
            free(tmp);
        } else ptr = &(*ptr)->next;
    }
}