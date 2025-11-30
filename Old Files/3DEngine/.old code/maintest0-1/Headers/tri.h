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

#ifdef SOKOL_IMPL
cube3d createCube(vec3 *vertices, int *order, vec4 *colours, cube3d *out) {
    for (int i = 0; i < 36; i += 3) {
        out->triangles[(int)i / 3].v[0] = vertices[order[i + 0]];
        out->triangles[(int)i / 3].v[1] = vertices[order[i + 1]];
        out->triangles[(int)i / 3].v[2] = vertices[order[i + 2]];
        out->triangles[(int)i / 3].color = colours[(int)i / 3];
        // printf("Color: %.2f %.2f %.2f %.2f\n", 
        //     perspectiveTriangles[i].color.x,
        //     perspectiveTriangles[i].color.y,
        //     perspectiveTriangles[i].color.z,
        //     perspectiveTriangles[i].color.w);

    }
    return *out;
}

#endif 


#endif 