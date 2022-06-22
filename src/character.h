#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <stdio.h>

#include "matrix.h"


typedef struct {
    int x;
    int y;
    enum {
        TOP,
        RIGHT,
        BOT,
        LEFT
    } rotation;
} Position;

typedef struct {
    int hp;
    int atk;
    int atk_speed;
    int speed;
} InitStats;

typedef struct {
    char name[32];
    InitStats stats;
    matrix_t markov;
} EntityType;

typedef struct {
    int current_hp;
    State current_state;
    Position world_position;
    EntityType* type;
} Entity;



/**
 * @brief Creates a new entity.
 * 
 * @param position his position in the level.
 * @param state the state of the entity, 0 when created, maybe != 0 when loaded
 * @param type the type of entity created.
 * @return a pointer to the new entity created.
 */
Entity* new_entity(Position position, State state, EntityType type);

/**
 * @brief Loads an entity from a save file.
 * 
 * @param file the save file.
 * @return a pointer to the loaded entity.
 */
Entity* load_entity(FILE* file);

/**
 * @brief Frees an entity.
 * 
 * @param entity a pointer to the entity.
 */
void free_entity(Entity* entity);

/**
 * @brief Loads the types from a configuration file.
 * 
 * @param file the configuration file.
 */
void load_types(FILE* file);

/**
 * @brief Loads a type from a file.
 * 
 * @param file the configuration file.
 * @return the type.
 */
EntityType* load_type(FILE* file);

/**
 * @brief Frees types.
 */
void free_types();

/**
 * @brief Frees a type
 * 
 * @param type a pointer to a type.
 */
void free_type(EntityType* type);


#endif
