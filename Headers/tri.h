#ifndef TRI_H
#define TRI_H

#include "vector.h"

/*---Structs---*/

typedef struct triangle2d {
    vec2 v[3]; // Array of vec2 (x, y) for each vertex
    vec4 color;    // RGBA color for the triangle
    float z[3];
} tri2d;

typedef struct triangle3d {
    vec3 v[3]; // Array of vec3 (x, y, z) for each vertex
    vec4 color;    // RGBA color for the triangle
    vec3 normal;
} tri3d;

typedef struct cube {
    tri3d triangles[12]; // 12 triangles for a cube
} cube3d;

#endif 