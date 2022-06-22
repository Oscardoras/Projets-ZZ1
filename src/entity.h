#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <stdbool.h>

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
    char name[32];
    struct {
        int hp;
        int atk;
        int atk_speed;
        int speed;
    } stats;
    Matrix markov;
} EntityType;

typedef struct {
    EntityType* type;
    int hp;
    State state;
    Position position;
} Entity;


/**
 * @brief Creates a new entity.
 * 
 * @param position his position in the level.
 * @param state the state of the entity, 0 when created, maybe != 0 when loaded.
 * @param hp hp of the entity created. if 0, takes type->state.hp instead.
 * @param type a pointer to the type of the entity created.
 * @return the new entity created.
 */
Entity* new_entity(EntityType* type, State state, int hp, Position position);

/**
 * @brief Saves an entity in the save-file.
 * 
 * @param entity the entity to save.
 * @param file the save-file.
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
 * @brief Acts identically to strcmp.
 * 
 * @param str1 the 1st string to compare.
 * @param str2 the 2nd string to compare.
 * @return true is the 2 strings are identical, false otherwise.
 */
bool compare_str(char* str1, char* str2);

/**
 * @brief Searchs in entity_types for the type of a said name.
 * 
 * @param name the name of the type wanted.
 * @return a pointer to the entity.
 */
EntityType* search_type(char *name);

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
