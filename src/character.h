#ifndef __CHARACTER_H__
#define __CHARACTER_H__


typedef struct {
    int x;
    int y;
    int rotation;
} Position;

typedef struct {
    int hp;
    int atk;
    int atk_speed;
    int speed;
} InitStats;

typedef struct {
    InitStats stats;
    matrix_t behaviour;
    
} ToE; //Type of entity

typedef struct {
    int current_hp;
    int current_state;

    ToE* type;

    Position world_position;
} Entity;




#endif
