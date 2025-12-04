/*
 * @brief Name: Max McGill
 * @brief Project: Story Board
 * @brief Description: 3D Graphics Engine
 * @brief File: main.c
 * @brief Start Date: 30.11.25
 *
 */

/* Headers */
#include<stdlib.h>
#include<stdio.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

#include "error.h"

/* Definitions */

/* GLoBAL Declarables*/
GLFWwindow* window;

/* Function Prototypes */
int cleanup(void);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void) {

    if (!glfwInit())
    {
        // Initialization failed
        ERRORS(-1, "Failed to initialize GLFW");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);


    window = glfwCreateWindow(640, 480, "Testing GLFW", NULL, NULL);
    if (!window) {
        ERRORS(-1, "Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        double time = glfwGetTime();
        glfwSwapBuffers(window);

        // printf("Time: %f seconds\n", time);
        getchar();
        break;
    }

    cleanup();
    return 0;
}

int cleanup(void) {
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}