/*
 * @brief Name: Max McGill
 * @brief Project: 3D Graphics Engine
 * @brief File: main.c
 * @brief Date: 27.06.25
 *
 */

 
#define SOKOL_IMPL
#define SOKOL_GLES3
#define SOKOL_NO_ENTRY

/*---Includes---*/
#include <stdio.h>
#include <stdlib.h>
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"


/*---Definitions---*/
#define WINDOWWIDTH 1280
#define WINDOWHEIGHT 720
#define BORDER 1

/*---Structs---*/


/*---Function Prototypes---*/



/*---GLoBAL Declarables---*/
sg_buffer vbuf;
sg_pipeline pip;
sg_bindings bind;
sg_image img;


/*---Vertex Shader---*/
const char* vs_src =
"#version 300 es\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec2 uv;\n"
"out vec2 frag_uv;\n"
"void main() {\n"
"  gl_Position = vec4(pos, 1.0);\n"
"  frag_uv = uv;\n"
"}\n";

const char* fs_src =
"#version 300 es\n"
"precision mediump float;\n"
"in vec2 frag_uv;\n"
"uniform sampler2D tex;\n"
"out vec4 frag_color;\n"
"void main() {\n"
"  frag_color = texture(tex, frag_uv);\n"
"}\n";

void init(void) {
    uint32_t pixels[4] = { 0xFFFFFFFF, 0xFF000000, 0xFF000000, 0xFFFFFFFF };
    img = sg_make_image(&(sg_image_desc){
        .width = 2,
        .height = 2,
        .pixel_format = SG_PIXELFORMAT_RGBA8,
        .data.subimage[0][0] = SG_RANGE(pixels)
    });

    float vertices[] = {
        // positions      // texcoords
        0.0f,  0.5f, 0.0f, 0.5f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    };

    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });

    vbuf = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(vertices)
    });

    sg_shader shd = sg_make_shader(&(sg_shader_desc){
        .vertex_func.source = vs_src,
        .fragment_func.source = fs_src
    });

    pip = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = shd,
        .layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3, // position
        .layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT2, // texcoord
    });

    bind.vertex_buffers[0] = vbuf;
    bind.images[0] = img;
}

void frame(void) {
    sg_pass pass = { 
        .action = {
            .colors[0] = {
                .load_action = SG_LOADACTION_CLEAR,
                .clear_value = { 0.1f, 0.1f, 0.1f, 1.0f }
            }  
        },
        .swapchain = sglue_swapchain()
    };
    sg_begin_pass(&pass);
    sg_apply_pipeline(pip);
    sg_apply_bindings(&bind);
    sg_draw(0, 3, 1);
    sg_end_pass();
    sg_commit();
}

/*---Main Function---*/
int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    sapp_desc desc = {
        .width = WINDOWWIDTH,
        .height = WINDOWHEIGHT,
        .window_title = "3D Graphics Engine",
        .init_cb = init,
        .frame_cb = frame,
    };

    sapp_run(&desc);
    return 0;
}

// sapp_desc sokol_main(int argc, char* argv[]) {
//     (void)argc; (void)argv;
//     return (sapp_desc){
//         .width = WINDOWWIDTH,
//         .height = WINDOWHEIGHT,
//         .window_title = "3D Graphics Engine",
//         .init_cb = init,
//         .frame_cb = frame,
//     };
// }
/*---Main Function---*/
