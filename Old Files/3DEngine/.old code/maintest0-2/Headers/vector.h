#ifndef VECTOR_H
#define VECTOR_H

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>




/*---Structs---*/
typedef struct vec2 {
    float x;
    float y;
} vec2;

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

typedef struct vec4 {
    float x;
    float y;
    float z;
    float w;
} vec4;


/*---Function Prototypes---*/
vec2 vec3ToVec2(vec3 *v);


/*---Functions---*/
vec2 vec3ToVec2(vec3 *v) {
    if (v == NULL) {
        fprintf(stderr, "Error: Null pointer argument.\n");
        return (vec2) {-999, -999};
    }
    return (vec2) { v-> x, v->y };
}

vec3 vecSub(vec3 v1, vec3 v2) {
    return (vec3) { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}
vec3 vecAdd(vec3 v1, vec3 v2) {
    return (vec3) { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

#endif