#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "entity.h"


typedef enum {
    AGGRESSIVE,
    DEFESIVE,
    NOT_GOING_OUT,
    GOING_OUT,
    TARGET_FOOD,
    TARGET_BUILD
} Behavior;
    
void change_rule(Level* level, Behavior behavior);


#endif