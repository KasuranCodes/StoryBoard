#ifndef PROJECTION_H
#define PROJECTION_H


#include "vector.h"
/*
    [x,y,z] -> [(h/w)fx, fy, z] (where f is 1/tan(theta/2))

    This is a simple perspective projection matrix->
    The matrix is in column-major order->

     z+ ^ 10___________________________ zfar
        |   \                         / ^
        |    \                       /  |
        |     \                     /   |
        |      \                   /    |
        |       \                 /     |
        |        \               /      |
        |         \             /       |
        |          \           /        |
        | 1         \_________/ znear   |
                               ^        |
                      \     /  |        |
                               |        |
                        \ /    |        |
                         o     |        |

    zfar * znear/(zfar - znear) = 10 / (10 - 1) = 10/9
    z * ( zfar / ( zfar - znear ) ) - ( ( zfar * znear ) / ( zfar - znear) )

    [ 
        ( ( w / h ) * ( 1 / tan( theta / 2 ) ) * x ) / z, 
        ( ( 1 / tan( theta / 2 ) ) * y ) / z, 
        z * ( zfar / ( zfar - znear ) ) - ( ( zfar * znear ) / ( zfar - znear) ) 
    ]
    
    [
        ( a * f * x ) / z,
        ( f * y ) / z,
        zq - znearq
    ]

    [ x, y, z ] -> [ ( afx ) / z, ( fy ) / z, zq - znearq ]

    where:
        a = w / h
        f = 1 / tan( theta / 2 )
        zq = z * ( zfar / ( zfar - znear ) )
        znearq = ( zfar * znear ) / ( zfar - znear )
        theta = field of view in radians
        w = width of the viewport
        h = height of the viewport

    matrix:
    [ x, y, z, 1 ] -> [ af, 0, 0,       0 ]
                      [ 0,  f, 0,       0 ]
                      [ 0,  0, q,       1 ]
                      [ 0,  0, -znearq, 0 ]
*/

/*---Structs---*/
typedef struct matrix_4x4 {
    float m[4][4]; // Column-major order
} mat4x4;


/*---Function Prototypes---*/
void createProjectionMatrix(mat4x4 *m, float fAspect, float fFovRad, float fNear, float fFar);
void multiplyMatrixVector(vec3 *in, vec3 *out, mat4x4 *m);

/*---GLoBAL Declarables---*/


/*---Create Projection Matrix---*/
void createProjectionMatrix(mat4x4 *m, float fAspect, float fFovRad, float fNear, float fFar) {
    if (m == NULL) {
        fprintf(stderr, "Error: Null pointer argument in createProjectionMatrix.\n");
        return;
    }
    /*
    
    mat4x4 m = {{
        { fAspect * fFovRad, 0.0f,             0.0f,                          0.0f },
        { 0.0f,              fFovRad,          0.0f,                          0.0f },
        { 0.0f,              0.0f,             fFar / (fFar - fNear),         1.0f },
        { 0.0f,              0.0f,            -(fFar * fNear) / (fFar - fNear), 0.0f }
    }};
    
    */
    
    
    /*---Video Perspective---*/
    m->m[0][0] = fAspect * fFovRad;
    m->m[0][1] = 0.0f;
    m->m[0][2] = 0.0f;
    m->m[0][3] = 0.0f;

    m->m[1][0] = 0.0f;
    m->m[1][1] = fFovRad;
    m->m[1][2] = 0.0f;
    m->m[1][3] = 0.0f;

    m->m[2][0] = 0.0f;
    m->m[2][1] = 0.0f;
    m->m[2][2] = fFar / (fFar - fNear);
    m->m[2][3] = 1.0f;

    m->m[3][0] = 0.0f;
    m->m[3][1] = 0.0f;
    m->m[3][2] = -(fFar * fNear) / (fFar - fNear);
    m->m[3][3] = 0.0f;
}


#endif