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
    char *name;
    InitStats stats;
    matrix_t markov;
} EntityType;

typedef struct {
    int current_hp;
    State current_state;
    Position world_position;

    EntityType* type;
} Entity;


extern EntityType entity_types[10];


/**
 * @brief Loads the types from a configuration file.
 * 
 * @param file the configuration file.
 */
void load_types(FILE* file);

/**
 * @brief Frees types.
 */
void free_types();

/**
 * @brief Loads a type from a file.
 * 
 * @param file the configuration file.
 * @return the type.
 */
EntityType load_type(FILE* file);

/**
 * @brief Frees a type
 * 
 * @param type a type.
 */
void free_type(EntityType* type);

/**
 * @brief Creates a new entity.
 * 
 * @param birth_position his position in the level.
 * @param type the type of entity created.
 * @return the new entity created.
 */
Entity* new_entity(Position birth_position, EntityType type);

/**
 * @brief Loads an entity from a save file.
 * 
 * @param file the save file.
 * @return the loaded entity, NULL if no entity has been loaded.
 */
Entity* load_entity(FILE* file);

/**
 * @brief Saves an entity into a save file.
 * 
 * @param entity an entity.
 * @param file the save file.
 */
void save_entity(Entity* entity, FILE* file);

/**
 * @brief Frees an entity.
 * 
 * @param entity the entity.
 */
void free_entity(Entity* entity);


#endif
