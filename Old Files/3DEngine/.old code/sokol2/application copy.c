/*
 * @brief Name: Max McGill
 * @brief Project: 3D Graphics Engine
 * @brief File: main.c
 * @brief Date: 02.06.25
 *
 */

#define SOKOL_IMPL
#define SOKOL_GLES3

/*---Includes---*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#include "tri.h"
#include "vector.h"
#include "camera.h"
#include "objimp.h"

#include "Shaders/tri_by_tri.glsl.h"


/*---Definitions---*/
#define WINDOWWIDTH 1280
#define WINDOWHEIGHT 720

/*---Structs---*/


/*---Function Prototypes---*/
uint64_t currentTimeMillis();
void init(void);
void cleanup(void);
void frame(void);


/*---Global Declarables---*/
int triNum = 0;
sg_buffer *vbufs; // Array of vertex buffers for triangles
sg_pipeline pip;
sg_bindings bind;

uint64_t startTime;
uint64_t totalTime;
uint64_t elapsedTime;
int currentFrame;

Camera cam; // Camera object for 3D perspective
vec3 cameraPosition = {0.0f, 0.0f, 0.0f}; // Camera position in 3D space
float cameraDirection = 0.0f; // Camera direction in degrees

tri3d *triQueue;
tri2d *perspectiveTriangles;
tri3d *rotatedTriangles;

cube3d cube;

vec3 cubeVertices[] = {
    { -0.5f, -0.5f, -0.5f },  // Vertex 0
    {  0.5f, -0.5f, -0.5f },  // Vertex 1
    {  0.5f,  0.5f, -0.5f },  // Vertex 2
    { -0.5f,  0.5f, -0.5f },  // Vertex 3 
    { -0.5f, -0.5f,  0.5f },  // Vertex 4
    {  0.5f, -0.5f,  0.5f },  // Vertex 5
    {  0.5f,  0.5f,  0.5f },  // Vertex 6
    { -0.5f,  0.5f,  0.5f }   // Vertex 7
};

int cubeOrder[] = {
    // Define indices for the cube's triangles
    0, 2, 1, 0, 3, 2, // Front face
    5, 6, 4, 4, 6, 7,  // Back face
    0, 1, 5, 0, 5, 4, // Bottom face
    2, 3, 7, 2, 7, 6, // Top face
    0, 7, 3, 0, 4, 7, // Left face
    1, 2, 6, 1, 6, 5 // Right face
};

vec4 cubeColors[] = {
    {1.0f, 0.0f, 0.0f, 1.0f},  // Red
    {1.0f, 0.0f, 0.0f, 1.0f},  // Red
    {0.0f, 1.0f, 0.0f, 1.0f},  // Green
    {0.0f, 1.0f, 0.0f, 1.0f},  // Green
    {0.0f, 0.0f, 1.0f, 1.0f},  // Blue
    {0.0f, 0.0f, 1.0f, 1.0f},  // Blue
    {1.0f, 1.0f, 0.0f, 1.0f},  // Yellow
    {1.0f, 1.0f, 0.0f, 1.0f},  // Yellow
    {1.0f, 0.0f, 1.0f, 1.0f},  // Magenta
    {1.0f, 0.0f, 1.0f, 1.0f},  // Magenta
    {0.0f, 1.0f, 1.0f, 1.0f},  // Cyan
    {0.0f, 1.0f, 1.0f, 1.0f}   // Cyan
};

// vec4 defaultColors[] = {
//     {1.0f, 0.0f, 0.0f, 1.0f},  // Red
//     {0.0f, 1.0f, 0.0f, 1.0f},  // Green
//     {0.0f, 0.0f, 1.0f, 1.0f},  // Blue
//     {1.0f, 1.0f, 0.0f, 1.0f},  // Yellow
//     {1.0f, 0.0f, 1.0f, 1.0f},  // Magenta
//     {0.0f, 1.0f, 1.0f, 1.0f}   // Cyan
// };

// vec4 cubeColors[] = {
//     {1.0f, 1.0f, 1.0f, 1.0f},  // Red
//     {1.0f, 1.0f, 1.0f, 1.0f},  // Red
//     {1.0f, 1.0f, 1.0f, 1.0f},  // Green
//     {1.0f, 1.0f, 1.0f, 1.0f},  // Green
//     {1.0f, 1.0f, 1.0f, 1.0f},  // Blue
//     {1.0f, 1.0f, 1.0f, 1.0f},  // Blue
//     {1.0f, 1.0f, 1.0f, 1.0f},  // Yellow
//     {1.0f, 1.0f, 1.0f, 1.0f},  // Yellow
//     {1.0f, 1.0f, 1.0f, 1.0f},  // Magenta
//     {1.0f, 1.0f, 1.0f, 1.0f},  // Magenta
//     {1.0f, 1.0f, 1.0f, 1.0f},  // Cyan
//     {1.0f, 1.0f, 1.0f, 1.0f}   // Cyan
// };


/*---Projection Matrix---*/
float fNear;
float fFar;
float fFov;
float fAspect;
float fFovRad;

mat4x4 projectionMatrix;


/*---Main Function---*/
sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    return (sapp_desc){
        .width = WINDOWWIDTH,
        .height = WINDOWHEIGHT,
        .window_title = "3D Graphics Engine",
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup
    };
}

uint64_t currentTimeMillis() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (uint64_t)(ts.tv_sec) * 1000 + (ts.tv_nsec) / 1000000;
}

void init(void) {
    // triNum = sizeof(triangles) / sizeof(tri3d);

    Object obj;
    loadObject("Models/blue42/blue42.obj", &obj);
    triNum = obj.triNum;
    triQueue = malloc(sizeof(tri3d) * triNum);

    // createCube(cubeVertices, cubeOrder, cubeColors, &cube);

    triQueue = obj.tris; 

    vbufs = malloc(sizeof(sg_buffer) * triNum); // Allocate memory for vertex buffers

    sg_setup(&(sg_desc){
        .logger.func = slog_func // Add this if you have sokol_log.h included
        
    });

    startTime = currentTimeMillis(); // Initialize start time
    totalTime = 0; // Initialize elapsed time
    currentFrame = 0;


    fNear = 5.0f;
    fFar = 1000.0f;
    fFov = 45.0f;
    fAspect = (float)WINDOWWIDTH / (float)WINDOWHEIGHT;
    fFovRad = 1.0f / tanf(RADS(fFov * 0.5f)); // Convert FOV to radians

    cam = cameraCreate(cameraPosition, cameraDirection, fFov, WINDOWWIDTH, WINDOWHEIGHT, fAspect, fNear, fFar);

    // Perspective transform triangles
    perspectiveTriangles = malloc(sizeof(tri2d) * triNum);
    rotatedTriangles = malloc(sizeof(tri3d) * triNum);


    for (int i = 0; i < triNum; i++) {
        vbufs[i] = sg_make_buffer(&(sg_buffer_desc){
            .label = "triangle-vertices",
            .size = sizeof(float) * 6,
            .usage = {
                .dynamic_update = true,
                .immutable = false
            }
        });
        assert(sg_query_buffer_state(vbufs[i]) == SG_RESOURCESTATE_VALID);

    }
    

    // Shader
    sg_shader shd = sg_make_shader(triangleshader_shader_desc(sg_query_backend()));
    if (shd.id == 0) {
        fprintf(stderr, "Shader creation failed\n");
        exit(1);
    }
    else {
        printf("Shader created successfully\n");
    }

    // Pipeline
    pip = sg_make_pipeline(&(sg_pipeline_desc){
        .layout.attrs[ATTR_triangleshader_position].format = SG_VERTEXFORMAT_FLOAT2,
        .shader = shd,
        .label = "triangle-pipeline"
    });
    if (pip.id == 0) {
        fprintf(stderr, "Pipeline creation failed\n");
        exit(1);
    }
    else {
        printf("Pipeline created successfully\n");
    }
    

}

void cleanup(void) {
    // Destroy vertex buffers
    for (int i = 0; i < triNum; i++) {
        if (vbufs[i].id != SG_INVALID_ID) {
            sg_destroy_buffer(vbufs[i]);
        }
    }

    // Free allocated memory
    free(vbufs);
    vbufs = NULL;

    free(perspectiveTriangles);
    perspectiveTriangles = NULL;


    // Destroy pipeline
    if (pip.id != SG_INVALID_ID) {
        sg_destroy_pipeline(pip);
    }

    // Shut down sokol_gfx
    sg_shutdown();

    printf("Cleanup complete.\n");
}

void frame(void) {
    int divisor = 50;
    int tempTriNum;
    totalTime = currentTimeMillis();
    if ((totalTime - startTime) / divisor > 360) {
        startTime = totalTime;
    }
    elapsedTime = (totalTime - startTime) / divisor;

    
    sg_begin_pass(&(sg_pass){
        .action = {
            .colors[0] = { 
                .load_action = SG_LOADACTION_CLEAR, 
                .clear_value = { 0.1f, 0.1f, 0.1f, 1.0f } 
            }
        },
        .swapchain = (sg_swapchain){
            .width = sapp_width(),
            .height = sapp_height(),
            .color_format = SG_PIXELFORMAT_RGBA8,
            .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
            .sample_count = 1
        },
        .label = "Main Pass"
    });
    
    // // Vertex buffer
    // for (int i = 0; i < triNum; i++) {
    //     rotateTri('y', elapsedTime, &triQueue[i], &rotatedTriangles[i]);
    //     rotateTri('x', -45, &rotatedTriangles[i], &rotatedTriangles[i]);
        
    // }
    // printf("triangles | %.2f, %.2f | %.2f, %.2f | %.2f, %.2f\n", triangles[0].v[0].x, triangles[0].v[0].y, triangles[0].v[1].x, triangles[0].v[1].y, triangles[0].v[2].x, triangles[0].v[2].y);
    renderPerspective(rotatedTriangles, &triNum, &tempTriNum, perspectiveTriangles, &cam);
    
    
    sg_apply_pipeline(pip);
    
    for (int i = 0; i < tempTriNum; i++) {
        
        sg_update_buffer(vbufs[i], &SG_RANGE(perspectiveTriangles[i].v));
        if (vbufs[i].id == SG_INVALID_ID) {
            fprintf(stderr, "Failed to create buffer %d\n", i);
        }
        
        sg_bindings bind = {0};
        bind.vertex_buffers[0] = vbufs[i];
        
        sg_apply_bindings(&bind);
        
        // printf("Color: %.2f %.2f %.2f %.2f\n", 
        //     perspectiveTriangles[i].color.x,
        //     perspectiveTriangles[i].color.y,
        //     perspectiveTriangles[i].color.z,
        //     perspectiveTriangles[i].color.w);

        sg_apply_uniforms(UB_u_color0, &SG_RANGE(perspectiveTriangles[i].color));
        
        // Draw the triangle
        sg_draw(0, 3, 1);
    }

    // printf("Elapsed |%5ld\n", elapsedTime); // Print the elapsed time in seconds



    // printf("Current Frame | %d\n", currentFrame / divisor);
    // currentFrame = (currentFrame != 360 * divisor) ? currentFrame + 1 : 0;
    currentFrame++;
    sg_end_pass();
    sg_commit();
}