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

typedef struct matrix_4x4 {
    float m[4][4]; // Column-major order
} mat4;

/*---Function Prototypes---*/
void multiplyMatrixVector(vec3 *in, vec3 *out, mat4 *m);
vec2 vec3ToVec2(vec3 *v);
vec3 vecSub(vec3 v1, vec3 v2);
vec3 vecAdd(vec3 v1, vec3 v2);


/*---Functions---*/
/*---Multiply Matrix by Vector---*/
void multiplyMatrixVector(vec3 *in, vec3 *out, mat4 *m) {
    out->x =     m->m[0][0] * in->x + m->m[1][0] * in->y + m->m[2][0] * in->z + m->m[3][0];
    out->y =     m->m[0][1] * in->x + m->m[1][1] * in->y + m->m[2][1] * in->z + m->m[3][1];
    out->z =     m->m[0][2] * in->x + m->m[1][2] * in->y + m->m[2][2] * in->z + m->m[3][2];
    float w =   m->m[0][3] * in->x + m->m[1][3] * in->y + m->m[2][3] * in->z + m->m[3][3];

    if (w != 0.0f) {
        out->x /= w;
        out->y /= w;
        out->z /= w;
    }
}

/*---Convert Vec3 to Vec2---*/
vec2 vec3ToVec2(vec3 *v) {
    if (v == ((void *)0)) {
        fprintf(((void *)0), "Error: Null pointer argument.\n");
        return (vec2) {-999, -999};
    }
    return (vec2) { v-> x, v->y };
}
/*---Subtract Two Vectors---*/
vec3 vecSub(vec3 v1, vec3 v2) {
    return (vec3) { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

/*---Add Two Vectors---*/
vec3 vecAdd(vec3 v1, vec3 v2) {
    return (vec3) { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}


#endif