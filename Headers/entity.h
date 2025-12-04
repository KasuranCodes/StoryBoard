#ifndef ENTITY_H
#define ENTITY_H


#include "vector.h"
#include "tri.h"
#include "objimp.h"

typedef struct entity {
    vec3 position;
    vec3 orientation;
    
    Object object;
} Entity;


#endif

