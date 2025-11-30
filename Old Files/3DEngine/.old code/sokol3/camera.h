#ifndef CAMERA_H
#define CAMERA_H


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tri.h"
#include "vector.h"
#include "projection.h"

/*---Definitions---*/
#define PI 3.14159265358979323846f
#define TRI_VERTICES 3
#define RADS(x) ((x) * (PI / 180.0f))
#define OFFSET 5


/*---Structs---*/
typedef struct camera {
    vec3 position;         // Camera position in 3D space
    vec3 direction;       // Direction the camera is facing
    float fov;             // Field of view in degrees
    float screenWidth;    // Width of the viewport
    float screenHeight;   // Height of the viewport
    float aspectRatio;     // Aspect ratio of the viewport
    float nearPlane;       // Near clipping plane distance
    float farPlane;        // Far clipping plane distance
    mat4x4 projectionMatrix; // Projection matrix for the camera
    mat4x4 viewMatrix;

} Camera;



/*---Function Prototypes---*/
static inline Camera cameraCreate(vec3 position, vec3 direction, float fov, float screenWidth, float screenHeight, float aspectRatio, float nearPlane, float farPlane);

static void rotateVertex(char axis, float fTheta, vec3 *in, vec3 *out);
static void rotateTri(char axis, float fTheta, tri3d *in, tri3d *out);

static void renderPerspective(tri3d *in, int *triNum, int *newNum, tri2d *out, Camera *cam);

static vec3 normalCalc(tri3d *in);
static float dotProductCalc(vec3 *a, vec3 *b);
static vec3 normalize(vec3 in);

float averageZ(tri2d t);
void quickSortRecursive(tri2d *arr, int low, int high);
int partition(tri2d *arr, int low, int high);
void quickSortTriangles(int size, tri2d *input, tri2d *output);

void updateViewMatrix(Camera *cam);
void lookAt(mat4x4 *mat, vec3 eye, vec3 center, vec3 up);
vec3 crossVec3(vec3 a, vec3 b);

/*---Functions---*/
static inline Camera cameraCreate(vec3 position, vec3 direction, float fov, float screenWidth, float screenHeight, float aspectRatio, float nearPlane, float farPlane) {
    Camera cam;
    cam.position = position;
    cam.direction = direction;
    cam.fov = fov;
    cam.screenWidth = screenWidth;
    cam.screenHeight = screenHeight;
    cam.aspectRatio = aspectRatio;
    cam.nearPlane = nearPlane;
    cam.farPlane = farPlane;
    createProjectionMatrix(&cam.projectionMatrix, aspectRatio, 1.0f / tanf(fov * 0.5f * (3.14159265358979323846f / 180.0f)), nearPlane, farPlane);
    return cam;
}

/*
    Translates a vec3 around an axis
    It takes vectors as input so when using with tris make sure to copy the tri colour over.
*/
static void rotateVertex(char axis, float fTheta, vec3 *in, vec3 *out) {
    if (in == NULL) {
        fprintf(stderr, "Error: Null pointer argument in rotateVertex.\n");
        return;
    }
    else if (axis != 'x' && axis != 'y' && axis != 'z') {
        fprintf(stderr, "Error: Invalid axis.\n");
        return;
    }
    fTheta = RADS(fTheta);
    switch (axis)
    {
        case 'x': {
            // Rotation around X-axis
            mat4x4 matRotx = {{
                {1, 0,           0,          0},
                {0, cosf(fTheta * 0.5), sinf(fTheta * 0.5), 0},
                {0, -sinf(fTheta * 0.5),  cosf(fTheta * 0.5), 0},
                {0, 0,           0,          1}
            }};
            multiplyMatrixVector(in, out, &matRotx);
            break;
        }

        case 'y': {
            // Rotation around Y-axis
            mat4x4 matRoty = {{
                { cosf(fTheta), 0, sinf(fTheta), 0},
                { 0,           1, 0,           0},
                {-sinf(fTheta), 0, cosf(fTheta), 0},
                { 0,           0, 0,           1}
            }};
            multiplyMatrixVector(in, out, &matRoty);
            break;
        }

        case 'z': {
            // Rotation around Z-axis
            mat4x4 matRotz = {{
                {cosf(fTheta), sinf(fTheta), 0, 0},
                {-sinf(fTheta),  cosf(fTheta), 0, 0},
                {0,            0,            1, 0},
                {0,            0,            0, 1}
            }};
            multiplyMatrixVector(in, out, &matRotz);
            break;
        }

        default:
            fprintf(stderr, "Invalid axis: %c. Use 'x', 'y', or 'z'.\n", axis);
            break;
    }

}

static void rotateTri(char axis, float fTheta, tri3d *in, tri3d *out) {
    rotateVertex(axis, fTheta, &in->v[0], &out->v[0]);
    rotateVertex(axis, fTheta, &in->v[1], &out->v[1]);
    rotateVertex(axis, fTheta, &in->v[2], &out->v[2]);
    out->color = in->color;
}

// static void renderPerspective(tri3d *in, int *triNum, int *newNum, tri2d *out, Camera *cam) {
//     if (in == NULL || out == NULL || cam == NULL) {
//         fprintf(stderr, "Error: Null pointer argument in renderPerspective.\n");
//         return;
//     }

//     memset(out, 0, sizeof(tri2d) * (*triNum));
//     tri3d *projectedTriangles = (tri3d *)malloc(sizeof(tri3d) * (*triNum));
//     if (projectedTriangles == NULL) {
//         fprintf(stderr, "Error: Memory allocation failed in renderPerspective.\n");
//         return;
//     }

//     int currentVal = 0;

//     for (int i = 0; i < *triNum; i++) {
//         normalCalc(&in[i]);

//         // Compute camera-to-triangle vector (world space)
//         vec3 camToTri = {
//             in[i].v[0].x - cam->position.x,
//             in[i].v[0].y - cam->position.y,
//             in[i].v[0].z + OFFSET - cam->position.z
//         };

//         float dp = dotProductCalc(&in[i].normal, &camToTri);
//         if (dp < 0.0f) {  // Triangle is facing the camera
//             int shouldRender = True;

//             for (int j = 0; j < 3; j++) {
//                 vec3 vertexWorld = in[i].v[j];
//                 vec3 vertexView;

//                 // Transform from world to view space
//                 multiplyMatrixVector(&vertexWorld, &vertexView, &cam->viewMatrix);

//                 // Optional Z offset for clipping bias
//                 vertexView.z += OFFSET;

//                 // Backface or behind near plane
//                 if (vertexView.z <= 0.01f) {
//                     shouldRender = False;
//                     break;
//                 }

//                 // Project to clip space
//                 vec3 vertexProjected;
//                 multiplyMatrixVector(&vertexView, &vertexProjected, &cam->projectionMatrix);

//                 // Perspective divide and screen-space transform
//                 vertexProjected.x = (vertexProjected.x + 1.0f) * 0.5f * cam->screenWidth;
//                 vertexProjected.y = (1.0f - vertexProjected.y) * 0.5f * cam->screenHeight;

//                 // Store projected vertex
//                 projectedTriangles[currentVal].v[j] = vertexProjected;
//                 out[currentVal].v[j] = vec3ToVec2(&vertexProjected);
//                 out[currentVal].z[j] = vertexProjected.z;
//             }

//             if (shouldRender) {
//                 out[currentVal].color = in[i].color;
//                 currentVal++;
//             }
//         }
//     }

//     *newNum = currentVal;
//     free(projectedTriangles);
// }


static void renderPerspective(tri3d *in, int *triNum, int *newNum, tri2d *out, Camera *cam) {
    if (in == NULL || out == NULL || cam == NULL) {
        fprintf(stderr, "Error: Null pointer argument in renderPerspective.\n");
        return;
    }
    memset(out, 0, sizeof(tri2d) * (*triNum));
    updateViewMatrix(cam);

    // Add this once before your main loop
    float yaw = RADS(cam->direction.y);   // Horizontal rotation (Y axis)
    float pitch = RADS(cam->direction.x); // Vertical rotation (X axis)

    // Optional: precompute sin/cos
    float cosYaw = cosf(yaw);
    float sinYaw = sinf(yaw);
    float cosPitch = cosf(pitch);
    float sinPitch = sinf(pitch);

    tri3d *projectedTriangles;
    projectedTriangles = (tri3d *)malloc(sizeof(tri3d) * *triNum);
    if (projectedTriangles == NULL) {
        fprintf(stderr, "Error: Memory allocation failed in renderPerspective.\n");
        return;
    }

    int currentVal = 0;
    for (int i = 0; i < *triNum; i++) {
        // printf("Tri %3d | Tris Rendered | %3d\n", i, currentVal);
        /* Translate tris */

        normalCalc(&in[i]);
        vec3 camToTri = {
            in[i].v[0].x - cam->position.x,
            in[i].v[0].y - cam->position.y,
            in[i].v[0].z + OFFSET - cam->position.z // offset z as you do
        };

        
        float dp = dotProductCalc(&in[i].normal, &camToTri);
        if (dp < 0.0f) {  // Triangle is facing the camera
            for (int j = 0; j < 3; j++) {

            //     if ((in[i].v[0].z - cam->position.z + OFFSET > 0) && (in[i].v[1].z - cam->position.z + OFFSET > 0) && (in[i].v[2].z - cam->position.z + OFFSET > 0)) {
            //         // Translate vertex relative to camera position
            //         vec3 temp = vecSub(in[i].v[j], cam->position);
            //         temp.z += OFFSET;

            //         // Rotate around X (pitch)
            //         vec3 rotatedX = {
            //             temp.x,
            //             temp.y * cosPitch - temp.z * sinPitch,
            //             temp.y * sinPitch + temp.z * cosPitch
            //         };

            //         // Rotate around Y (yaw)
            //         vec3 rotated = {
            //             rotatedX.x * cosYaw + rotatedX.z * sinYaw,
            //             rotatedX.y,
            //             -rotatedX.x * sinYaw + rotatedX.z * cosYaw
            //         };

            //         multiplyMatrixVector(&rotated, &projectedTriangles[i].v[j], &cam->projectionMatrix);

            //         projectedTriangles[i].v[j].x = (projectedTriangles[i].v[j].x / cam->screenWidth) - 1.0f;
            //         projectedTriangles[i].v[j].y = 1.0f - (projectedTriangles[i].v[j].y / cam->screenHeight) * 2.0f;
            //         projectedTriangles[i].v[j].x = (projectedTriangles[i].v[j].x + 1.0f) * 0.5f * cam->screenWidth;
            //         projectedTriangles[i].v[j].y = (1.0f - projectedTriangles[i].v[j].y) * 0.6f * cam->screenHeight;

            //         out[currentVal].v[j] = vec3ToVec2(&projectedTriangles[i].v[j]);
            //         out[currentVal].z[j] = projectedTriangles[i].v[j].z;
            //     }

             // Vertex in world space
                vec3 vertexWorld = in[i].v[j];

                // Transform vertex to view space
                vec3 vertexView;
                multiplyMatrixVector(&vertexWorld, &vertexView, &cam->viewMatrix);

                // Z offset for visibility
                vertexView.z += OFFSET;

                // Cull behind camera
                if (vertexView.z <= 0.1f) continue;

                // Transform vertex to projection space
                vec3 vertexProjected;
                multiplyMatrixVector(&vertexView, &vertexProjected, &cam->projectionMatrix);

                // Perspective divide and screen space transform
                vertexProjected.x = (vertexProjected.x + 1.0f) * 0.5f * cam->screenWidth;
                vertexProjected.y = (1.0f - vertexProjected.y) * 0.5f * cam->screenHeight;

                out[currentVal].v[j] = vec3ToVec2(&vertexProjected);
                out[currentVal].z[j] = vertexProjected.z;
            }
            

            out[currentVal].color = in[i].color;
            currentVal++;
        }

    }
    *newNum = currentVal;
    free(projectedTriangles);

}

static vec3 normalCalc(tri3d *in) {
    vec3 line1, line2;
    line1.x = in->v[1].x - in->v[0].x;
    line1.y = in->v[1].y - in->v[0].y;
    line1.z = in->v[1].z - in->v[0].z;

    line2.x = in->v[2].x - in->v[0].x;
    line2.y = in->v[2].y - in->v[0].y;
    line2.z = in->v[2].z - in->v[0].z;
    
    in->normal.x = line1.y * line2.z - line1.z * line2.y;
    in->normal.y = line1.z * line2.x - line1.x * line2.z;
    in->normal.z = line1.x * line2.y - line1.y * line2.x;

    in->normal = normalize(in->normal);
    
    return in->normal;
}

static float dotProductCalc(vec3 *a, vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

static vec3 normalize(vec3 in) {
    float l = sqrtf(in.x*in.x + in.y*in.y + in.z*in.z);
    
    return (vec3) {in.x / l, in.y / l, in.z / l};

}

float averageZ(tri2d t) {
    return (t.z[0] + t.z[1] + t.z[2]) / 3.0f;
}

/*
    QUICKSORT FUNCTION BASED ON Z VALUES
    ---
*/
void quickSortTriangles(int size, tri2d *in, tri2d *out) {
    memcpy(out, in, size * sizeof(tri2d)); // Copy input to output
    quickSortRecursive(out, 0, size - 1);  // Sort output in-place
}

// Partition function for quicksort
int partition(tri2d *arr, int low, int high) {
    float pivot = averageZ(arr[high]);
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (averageZ(arr[j]) < pivot) {
            i++;
            tri2d temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    tri2d temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

// Recursive quicksort on triangle array
void quickSortRecursive(tri2d *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortRecursive(arr, low, pi - 1);
        quickSortRecursive(arr, pi + 1, high);
    }
}

vec3 getForwardVector(Camera *cam) {
    float yaw = RADS(cam->direction.y);  // yaw: left/right
    float pitch = RADS(cam->direction.x); // pitch: up/down

    vec3 forward;
    forward.x = cosf(pitch) * sinf(yaw);
    forward.y = sinf(pitch);
    forward.z = cosf(pitch) * cosf(yaw);

    return forward;
}

vec3 getRightVector(Camera *cam) {
    float yaw = RADS(cam->direction.y);
    
    vec3 right;
    right.x = sinf(yaw - 3.14);
    right.y = 0;
    right.z = cosf(yaw - 3.14);
    
    return right;
}

void updateViewMatrix(Camera *cam) {
    float pitch = RADS(cam->direction.x);
    float yaw = RADS(cam->direction.y);

    vec3 forward = {
        cosf(pitch) * sinf(yaw),
        sinf(pitch),
        cosf(pitch) * cosf(yaw)
    };
    vec3 target = {
        cam->position.x + forward.x,
        cam->position.y + forward.y,
        cam->position.z + forward.z
    };

    vec3 up = {0.0f, 1.0f, 0.0f};

    lookAt(&cam->viewMatrix, cam->position, target, up);
}

void lookAt(mat4x4 *mat, vec3 eye, vec3 center, vec3 up) {
    vec3 f = normalize(vecSub(center, eye));
    vec3 s = normalize(crossVec3(f, up));
    vec3 u = crossVec3(s, f);

    *mat = (mat4x4){{
        { s.x,  u.x, -f.x, 0.0f },
        { s.y,  u.y, -f.y, 0.0f },
        { s.z,  u.z, -f.z, 0.0f },
        {
            -dotProductCalc(&s, &eye),
            -dotProductCalc(&u, &eye),
             dotProductCalc(&f, &eye),
             1.0f
        }
    }};
}

vec3 crossVec3(vec3 a, vec3 b) {
    vec3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}


#endif // CAMERA_H