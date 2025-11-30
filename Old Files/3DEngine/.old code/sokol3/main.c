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


/*---Shaders---*/
#include "Shaders/tri_by_tri.glsl.h"

/*---Definitions---*/
#define WINDOWWIDTH 1280
#define WINDOWHEIGHT 720

#define MODEL "Models/blue42/blue42.obj"

/*---Structs---*/


/*---Function Prototypes---*/
uint64_t currentTimeMillis();
void init(void);
void cleanup(void);
void frame(void);

/*---GLoBAL Declarables---*/
// Buffers
int triNum = 0;
sg_buffer vbufs; // Array of vertex buffers for triangles
sg_pipeline pip;
sg_bindings bind;

// Timing
uint64_t startTime;
uint64_t totalTime;
uint64_t elapsedTime;
int currentFrame;

// Camera
Camera cam; // Camera object for 3D perspective
vec3 cameraPosition = {0.0f, 0.0f, 0.0f}; // Camera position in 3D space
float cameraDirection = 0.0f; // Camera direction in degrees

// Tris
tri3d *triQueue;
tri3d *rotatedTriangles;
tri2d *perspectiveTriangles;
vec2 *totalTris;

// Projection Matrix
float fNear;
float fFar;
float fFov;
float fAspect;
float fFovRad;

mat4x4 projectionMatrix;

/*---Current Time---*/
uint64_t currentTimeMillis() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (uint64_t)(ts.tv_sec) * 1000 + (ts.tv_nsec) / 1000000;
}

/*---Initialization---*/
void init(void) {
    // Sokol Setup
    sg_setup(&(sg_desc){
        .logger.func = slog_func // Add this if you have sokol_log.h included
        
    });

    // Initialize Model
    Object obj;
    loadObject(MODEL, &obj);

    triNum = obj.triNum;
    triQueue = malloc(sizeof(tri3d) * triNum);
    triQueue = obj.tris;

    // Initialize Timing Variables
    startTime = currentTimeMillis(); // Initialize start time
    totalTime = 0; // Initialize elapsed time
    currentFrame = 0;

    // Initialize Camera
    fNear = 5.0f;
    fFar = 1000.0f;
    fFov = 45.0f;
    fAspect = (float)WINDOWWIDTH / (float)WINDOWHEIGHT;
    fFovRad = 1.0f / tanf(RADS(fFov * 0.5f)); // Convert FOV to radians

    cam = cameraCreate(cameraPosition, cameraDirection, fFov, WINDOWWIDTH, WINDOWHEIGHT, fAspect, fNear, fFar);

    // Initialize the Tri Variables
    rotatedTriangles = malloc(sizeof(tri3d) * triNum);
    perspectiveTriangles = malloc(sizeof(tri2d) * triNum);

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

/*---Cleanup---*/
void cleanup(void) {
    // Destroy vertex buffers
    for (int i = 0; i < triNum; i++) {
        if (vbufs.id != SG_INVALID_ID) {
            sg_destroy_buffer(vbufs);
        }
    }

    
    // Free allocated memory
    free(rotatedTriangles);
    rotatedTriangles = NULL;

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

/*---Frame Function---*/
void frame(void) {
    // Temporary Tri Num
    int tempTriNum = 0;

    // Divisor to set rotation speed
    int divisor = 50;

    // Time Set
    totalTime = currentTimeMillis();
    if ((totalTime - startTime) / divisor > 360) {
        startTime = totalTime;
    }
    elapsedTime = (totalTime - startTime) / divisor;

    // Begin the Pass
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

    for (int i = 0; i < triNum; i++) {
        rotateTri('y', elapsedTime, &triQueue[i], &rotatedTriangles[i]);
        rotateTri('x', -45, &rotatedTriangles[i], &rotatedTriangles[i]);
        
    }
    renderPerspective(rotatedTriangles, &triNum, &tempTriNum, perspectiveTriangles, &cam);
    if (tempTriNum <= 0) {
        printf("No triangles to draw this frame.\n");
        sg_end_pass();
        sg_commit();
        return;
    }

    totalTris = malloc(sizeof(vec2) * (tempTriNum * 3));
    for (int i = 0; i < tempTriNum; i += 3) {
        totalTris[i + 0] = perspectiveTriangles[i].v[0];
        totalTris[i + 2] = perspectiveTriangles[i].v[1];
        totalTris[i + 3] = perspectiveTriangles[i].v[2];
        printf("totalTris %d| %5.2f %5.2f | %5.2f %5.2f | %5.2f %5.2f \n", i / 3, totalTris[i + 0].x, totalTris[i + 0].y, totalTris[i + 1].x, totalTris[i + 1].y, totalTris[i + 2].x, totalTris[i + 2].y);
    }

    sg_apply_pipeline(pip);
    vbufs = sg_make_buffer(&(sg_buffer_desc){
        .size = sizeof(vec2) * tempTriNum * 3,
        .usage = {
            .dynamic_update = True,
            .immutable = False
        }
    });

    sg_bindings bind = {0};
    bind.vertex_buffers[0] = vbufs;
    
    sg_apply_bindings(&bind);
    // vbufs = _sg_create_buffer()
    sg_update_buffer(vbufs, &SG_RANGE(totalTris));
    if (vbufs.id == SG_INVALID_ID) {
        fprintf(stderr, "Failed to create buffer \n");
    }
    else {
        printf("Buffer created successfully\n");
    }


    for (int i = 0; i < tempTriNum; i++)
    {
        sg_apply_uniforms(UB_u_color0, &SG_RANGE(perspectiveTriangles[0].color));
        sg_draw(0, 3, 1);
    }
    
    
    currentFrame++;
    sg_end_pass();
    sg_commit();
}
