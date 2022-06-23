#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <stdbool.h>

#include "matrix.h"


typedef enum {
    TOP,
    RIGHT,
    BOT,
    LEFT
} Direction;

typedef struct {
    int x;
    int y;
    Direction direction;
} Position;

typedef struct {
    struct {
        int hp;
        int atk;
        int atk_speed;
        int speed;
    } stats;
    Matrix markov;
} EntityType;

#define ENTITY_TYPES 10
typedef enum {
    WORKER,
    SOLDIER,
    QUEEN,
    MANTIS,
    PHEROMONE,
    FOOD,
} EntityTypeName;
extern EntityType* entity_types[ENTITY_TYPES];

typedef struct Entity {
    EntityTypeName type;
    Position position;
    int hp;
    State state;
    struct Entity* target;
} Entity;



/**
 * @brief Creates a new entity.
 * 
 * @param type the type of the new entity.
 * @param position the position in the level.
 * @param hp health point of the new entity. If negative then takes the value of the type instead.
 * @param state the state of the entity.
 * @return the new entity.
 */
Entity* new_entity(EntityTypeName type, Position position, int hp, State state);

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
 * @brief Frees a type
 * 
 * @param type a pointer to a type.
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
 */
void free_types();


#endif
