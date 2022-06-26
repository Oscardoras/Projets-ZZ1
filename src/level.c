#include <stdlib.h>
#include <time.h>

#include "level.h"


Level* new_level(int min_x, int max_x, int min_y, int max_y, int seed) {
    Level* level = malloc(sizeof(Level));

    if (level != NULL) {
        unsigned int size = (max_x - min_x) * (max_y - min_y);
        level->blocks = malloc(sizeof(Block) * size);

        if (level->blocks != NULL) {
            level->d.min_x = min_x;
            level->d.max_x = max_x;
            level->d.min_y = min_y;
            level->d.max_y = max_y;
            level->seed = seed;
            level->states.score = 0;
            level->states.weather = WEATHER_SUN;
            level->states.behavior = BEHAVIOR_DEFAULT;
            level->states.spawn_speed = 0;
            level->states.food = 0;
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
        
        for (struct EntityListCell* it = level->entities; it != NULL;) {
            struct EntityListCell* tmp = it;
            it = it->next;
            free_entity(tmp->entity);
            free(tmp);
        }
        
        free(level);
    }
}

void save_level(Level* level, FILE* file) {
    fprintf(file, "%d %d %d %d %d %d %d %d %d %d\n",
        level->d.min_x, level->d.max_x, level->d.min_y, level->d.max_y,
        level->seed,
        level->states.score, level->states.weather, level->states.behavior, level->states.spawn_speed, level->states.food
    );
    
    unsigned int size = (level->d.max_x - level->d.min_x) * (level->d.max_y - level->d.min_y);
    for (unsigned int k = 0; k < size; k++)
        fprintf(file, "%d\n", level->blocks[k]);
    
    for (struct EntityListCell* it = level->entities; it != NULL; it = it->next)
        save_entity(it->entity, file);
}

Level* load_level(FILE* file) {
    Level* level = malloc(sizeof(Level));
    
    if (level != NULL) {
        int n = fscanf(file, "%d %d %d %d %d %d %d %d %d %d",
            &level->d.min_x, &level->d.max_x, &level->d.min_y, &level->d.max_y,
            &level->seed,
            &level->states.score, &level->states.weather, &level->states.behavior, &level->states.spawn_speed, &level->states.food
        );

        if (n == 10) {
            unsigned int size = (level->d.max_x - level->d.min_x) * (level->d.max_y - level->d.min_y);
            level->blocks = malloc(sizeof(Level) * size);

            if (level->blocks != NULL) {
                for (unsigned int i = 0; i < size; i++) {
                    n = fscanf(file, "%d", &level->blocks[i]);
                    if (n != 1) {
                        free(level->blocks);
                        free(level);
                        return NULL;
                    }
                }
            
                Entity* entity;
                while ((entity = load_entity(file)) != NULL)
                    add_level_entity(level, entity);
            } else {
                free(level);
                level = NULL;
            }
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
    else return NULL;
}

bool add_level_entity(Level* level, Entity* entity) {
    struct EntityListCell* it = malloc(sizeof(struct EntityListCell));
    if (it != NULL) {
        it->entity = entity;
        it->next = level->entities;
        level->entities = it;
        return true;
    } else return false;
}

void clean_level_entities(Level* level) {
    for (struct EntityListCell* it = level->entities; it != NULL; it = it->next)
        if (it->entity->target != NULL && it->entity->target->state == 0)
            it->entity->target = NULL;
    
    for (struct EntityListCell** ptr = &level->entities; *ptr != NULL;) {
        if ((*ptr)->entity->state == 0) {
            struct EntityListCell* tmp = *ptr;
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
                if (y < height) *b = BLOCK_DIRT;
                else if (y > height) *b = BLOCK_AIR;
                else *b = BLOCK_GRASS;
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
                if (y < height) *b = BLOCK_DIRT;
                else if (y > height) *b = BLOCK_AIR;
                else *b = BLOCK_GRASS;
            }
        }
    }
    
    if (min_x <= 0 && 0 < max_x) {
        for (int y = min_y; y < max_y; y++) {
            Block* b = get_level_block(level, 0, y);
            if (y < 0) *b = BLOCK_DIRT;
            else if (y > 0) *b = BLOCK_AIR;
            else *b = BLOCK_GRASS;
        }
    }
    
    srand(time(NULL));
}