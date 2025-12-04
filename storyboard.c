/*
 * @brief Name: Max McGill
 * @brief Project: Story Board
 * @brief Description: 3D Graphics Engine
 * @brief File: main.c
 * @brief Start Date: 30.11.25
 *
 */

/* Headers */
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "error.h"

#include "src/main.c"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


/* Definitions */

/* GLoBAL Declarables*/
GLFWwindow* window;


static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";
 



/* Function Prototypes */
int cleanup();
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void error_callback(int error, const char* description);




int main(void) 
{

    #ifndef CAM
    ERRORS(-1, "No camera defined! Create a camera in main.c before proceeding.");
    #endif


    if (!glfwInit())
    {
        // Initialization failed
        ERRORS(-1, "Failed to initialize GLFW");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_FALSE);
    
    window = glfwCreateWindow(640, 480, "Testing GLFW", NULL, NULL);
    if (!window) 
    {
        ERRORS(-1, "Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetErrorCallback(error_callback);
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);
    
    init();
    
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        // printf("Width: %d, Height: %d\n", width, height);
    
        glViewport(0, 0, width, height);

        double time = glfwGetTime();
        
        glUseProgram(program);
        frame();

        glfwSwapBuffers(window);
        
        glfwPollEvents();
        // printf("Time: %f seconds\n", time);
    }

    cleanup();
    return 0;
}

int cleanup(void) 
{
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action = GLFW_PRESS) 
    {
        printf("Key Pressed: key=%d, scancode=%d, mods=%d\n", key, scancode, mods);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "glfw error(%d): %s\n", error, description);
}
