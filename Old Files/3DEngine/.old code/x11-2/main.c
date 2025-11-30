#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "tri.h"
#include "vector.h"
#include "camera.h"
#include "objimp.h"

/*---Definitions---*/
#define WINDOWWIDTH 1280
#define WINDOWHEIGHT 720

#define MODEL "Models/test/thingy.obj"
// #define MODEL "Models/blue42/blue42.obj"


/*---Prototype Functions---*/
uint64_t currentTimeMillis();
int init(void);
int frame(void);
int drawTri(vec2*, vec4);

/*---GLoBAL Declarables---*/
// Window Shit
int windowHeight = WINDOWHEIGHT;
int windowWidth = WINDOWWIDTH;

// Tris
int triNum = 0;
tri3d *triQueue;
tri3d *rotatedTriangles;
tri2d *perspectiveTriangles;
vec2 *totalTris;


// Camera
Camera cam; // Camera object for 3D perspective
vec3 cameraPosition = {0.0f, 0.0f, -10.0f}; // Camera position in 3D space
vec3 cameraDirection = {0.0f, 0.0f, 0.0f}; // Camera direction in degrees

float fNear;
float fFar;
float fFov;
float fAspect;
float fFovRad;

mat4x4 projectionMatrix;

// Timing
uint64_t startTime;
uint64_t totalTime;
uint64_t elapsedTime;
int currentFrame;

// Test Tri
tri3d test = {
    .color = {0.588, 0.482, 0.714, 1.0f},
    .v = {  
        { -0.5f, -0.5f,  0.0f },  
        {  0.5f, -0.5f, 10.0f },  
        { -0.5f,  0.5f,  0.0f } 
    }
};


/*---Current Time---*/
uint64_t currentTimeMillis() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (uint64_t)(ts.tv_sec) * 1000 + (ts.tv_nsec) / 1000000;
}


/*---Draw Tri---*/
/*
 * This draws a tri using openGL
 * It takes a vec3 color scheme and an array of vec3 position coordinates
 * It returns a success code
 * 
 * -------------------------
 * 0    | Failure
 * 1    | Success
 */
int drawTri(vec2 *position, vec4 colour) {
    glBegin(GL_TRIANGLES);
    glColor4f(colour.x, colour.y, colour.z, colour.w); glVertex2f(position[0].x, position[0].y);
    glColor4f(colour.x, colour.y, colour.z, colour.w); glVertex2f(position[1].x, position[1].y);
    glColor4f(colour.x, colour.y, colour.z, colour.w); glVertex2f(position[2].x, position[2].y);
    glEnd();
    return 1;
}

/*---Initializer---*/
int init(void) {
    // Initialize Timing Variables
    startTime = currentTimeMillis(); // Initialize start time
    totalTime = 0; // Initialize elapsed time
    currentFrame = 0;


    // Initialize Model
    Object obj;
    loadObject(MODEL, &obj);

    triNum = obj.triNum;
    triQueue = malloc(sizeof(tri3d) * triNum);
    triQueue = obj.tris;

    // Initialize Camera
    fNear = 5.0f;
    fFar = 1000.0f;
    fFov = 45.0f;
    fAspect = (float)windowWidth / (float)windowHeight;
    fFovRad = 1.0f / tanf(RADS(fFov * 0.5f)); // Convert FOV to radians

    cam = cameraCreate(cameraPosition, cameraDirection, fFov, windowWidth, windowHeight, fAspect, fNear, fFar);

    // Initialize the Tri Variables
    rotatedTriangles = malloc(sizeof(tri3d) * triNum);
    perspectiveTriangles = malloc(sizeof(tri2d) * triNum);

    return 1;
}

/*---Frame---*/
/*
 *  This is the function that is called every frame
 *
 *  It returns the current frame number
 */
int frame(void) {
    // Clear last Frame
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Divisor to set rotation speed
    int divisor = 50;

    // Time Set
    totalTime = currentTimeMillis();
    if ((totalTime - startTime) / divisor > 360) {
        startTime = totalTime;
    }
    elapsedTime = (totalTime - startTime) / divisor;
    // printf("elapsedtime\t| %d\n", elapsedTime);

    int tempTriNum = 0;
    
    for (int i = 0; i < triNum; i++) {
        rotateTri('y', elapsedTime, &triQueue[i], &rotatedTriangles[i]);
    }
    renderPerspective(rotatedTriangles, triNum, &tempTriNum, perspectiveTriangles, &cam);
    // printf("tempTriNum\t|\t\t%d\n", tempTriNum);

    quickSortTriangles(tempTriNum, perspectiveTriangles, perspectiveTriangles);

    for (int i = 0; i < tempTriNum; i++) {
        drawTri(perspectiveTriangles[i].v, perspectiveTriangles[i].color);
    }
    return 1;
}