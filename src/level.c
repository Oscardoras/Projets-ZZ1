#include <stdlib.h>
#include <time.h>

#include "level.h"


Level* new_level(int min_x, int max_x, int min_y, int max_y, int seed) {
    Level* level = malloc(sizeof(Level));
    if (level != NULL) {
        int size = (max_x - min_x) * (max_y - min_y);
        level->blocks = malloc(sizeof(Block) * size);
        if (level->blocks != NULL) {
            level->d.min_x = min_x;
            level->d.max_x = max_x;
            level->d.min_y = min_y;
            level->d.max_y = max_y;
            level->seed = seed;
            level->states.weather = SUN;
            level->states.behavior = DEFAULT;
            level->entities = NULL;
            
            generate_level(level, min_x, max_x, min_y, max_y, seed);
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
    fprintf(file, "%d %d %d %d %d %d %d\n",
        level->d.min_x, level->d.max_x, level->d.min_y, level->d.max_y,
        level->seed,
        level->states.weather, level->states.behavior
    );
    
    int size = (level->d.max_x - level->d.min_x) * (level->d.max_y - level->d.min_y);
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
        level->d.min_x = atoi(value);
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->d.max_x = atoi(value);
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->d.min_y = atoi(value);
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->d.max_y = atoi(value);
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->seed = atoi(value);
        
        for (v = value; *b != ' '; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->states.weather = atoi(value);
        
        for (v = value; *b != '\n'; b++) {
            *v = *b;
            v++;
        }
        b++;
        *v = '\0';
        level->states.behavior = atoi(value);
        
        int size = (level->d.max_x - level->d.min_x) * (level->d.max_y - level->d.min_y);
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
        for (int x = level2->d.min_x; x < level2->d.max_x; x++) {
            for (int y = level2->d.min_y; y < level2->d.max_y; y++) {
                Block* b = get_level_block(level, x, y);
                Block* b2 = get_level_block(level2, x, y);
                if (b != NULL)
                    *b2 = *b;
                else {
                    if (level2->d.min_x < level->d.min_x)
                        generate_level(level, level2->d.min_x, level->d.min_x, level->d.min_y, level->d.max_y, level->seed);
                    if (level->d.max_x < level2->d.max_x)
                        generate_level(level, level->d.max_x, level2->d.max_x, level->d.min_y, level->d.max_y, level->seed);
                    if (level2->d.min_y < level->d.min_y)
                        generate_level(level, level2->d.min_x, level2->d.max_x, level2->d.min_y, level->d.min_y, level->seed);
                    if (level->d.max_y < level2->d.max_y)
                        generate_level(level, level2->d.min_x, level2->d.max_x, level->d.max_y, level2->d.max_y, level->seed);
                }
            }
        }
        
        free(level->blocks);
        *level = *level2;
        return true;
    } return false;
}

Block* get_level_block(Level* level, int x, int y) {
    if (level->d.min_x <= x && x < level->d.max_x && level->d.min_y <= y && y < level->d.max_y)
        return &level->blocks[(y - level->d.min_y)*(level->d.max_x - level->d.min_x) + (x - level->d.min_x)];
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
    for (struct ListCell* cell = level->entities; cell != NULL; cell = cell->next)
        if (cell->entity->target != NULL && cell->entity->target->state == 0)
            cell->entity->target = NULL;
    
    for (struct ListCell** ptr = &level->entities; *ptr != NULL;) {
        if ((*ptr)->entity->state == 0) {
            struct ListCell* tmp = *ptr;
            *ptr = (*ptr)->next;
            free(tmp);
        } else ptr = &(*ptr)->next;
    }
}

void generate_level(Level* level, int min_x, int max_x, int min_y, int max_y, int seed) {
    int height;
    
    height = 0;
    for (int x = 1; x < max_x; x++) {
        srand(seed + x);
        int r = rand()%10;
        if (r < 1) height -= 2;
        else if (r < 3) height--;
        else if (r < 5) height++;
        else if (r < 6) height += 2;
        
        if (min_x <= x) {
            for (int y = min_y; y < max_y; y++) {
                Block* b = get_level_block(level, x, y);
                if (y < height) *b = DIRT;
                else if (y > height) *b = AIR;
                else *b = GRASS;
            }
        }
    }
    
    height = 0;
    for (int x = -1; min_x <= x; x--) {
        srand(seed + x);
        int r = rand()%10;
        if (r < 1) height -= 2;
        else if (r < 3) height--;
        else if (r < 5) height++;
        else if (r < 6) height += 2;
        
        if (x < max_x) {
            for (int y = min_y; y < max_y; y++) {
                Block* b = get_level_block(level, x, y);
                if (y < height) *b = DIRT;
                else if (y > height) *b = AIR;
                else *b = GRASS;
            }
        }
    }
    
    if (min_x <= 0 && 0 < max_x) {
        for (int y = min_y; y < max_y; y++) {
            Block* b = get_level_block(level, 0, y);
            if (y < 0) *b = DIRT;
            else if (y > 0) *b = AIR;
            else *b = GRASS;
        }
    }
    
    srand(time(NULL));
}