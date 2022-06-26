#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <stdbool.h>

#include "markov.h"


typedef enum {
    DIRECTION_LEFT,
    DIRECTION_TOP,
    DIRECTION_RIGHT,
    DIRECTION_BOT
} Direction;

/**
 * @brief Represents the location of an entity.
 * 
 */
typedef struct {
    int x;
    int y;
    Direction direction;
} Location;

/**
 * @brief Stores data about an entity type.
 * 
 */
typedef struct {
    struct {
        int hp;
        int atk;
    } stats;
    Matrix* markov;
} EntityType;

typedef enum {
    ENTITY_WORKER,
    ENTITY_SOLDIER,
    ENTITY_QUEEN,
    ENTITY_MANTIS,
    ENTITY_PHEROMONE,
    ENTITY_FOOD,
} EntityTypeName;
#define ENTITY_TYPES 6

/**
 * @brief The list of entity types.
 * 
 */
extern EntityType* entity_types[ENTITY_TYPES];

/**
 * @brief Represents an entity.
 * 
 */
typedef struct Entity {
    EntityTypeName type;
    Location location;
    State state;
    struct Entity* target;
    int hp;
} Entity;



/**
 * @brief Creates a new entity.
 * 
 * @param type the type of the new entity.
 * @param location the location in the level.
 * @param state the state of the entity.
 * @return the new entity.
 */
Entity* new_entity(EntityTypeName type, Location location, State state);

/**
 * @brief Saves an entity in a save file.
 * 
 * @param entity the entity to save.
 * @param file the save file.
 */
void save_entity(Entity* entity, FILE* file);

/**
 * @brief Loads an entity from a save file.
 * 
 * @param file the save file.
 * @return the loaded entity, NULL if no entity has been loaded.
 */
Entity* load_entity(FILE* file);

/**
 * @brief Frees an entity.
 * 
 * @param entity the entity.
 */
void free_entity(Entity* entity);

/**
 * @brief Loads a type from a configuration file.
 * 
 * @param file the configuration file.
 * @return the type.
 */
EntityType* load_type(FILE* file);

/**
 * @brief Frees a type.
 * 
 * @param type the type.
 */
void free_type(EntityType* type);

/**
 * @brief Loads the types from a configuration file.
 * 
 * @param file the configuration file.
 */
void load_types(FILE* file);

/**
 * @brief Frees types.
 * 
 */
void free_types();


#endif
