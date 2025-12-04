#ifndef CONTEXT_H
#define CONTEXT_H

#include "camera.h"

typedef struct context {
    int menuState;
    int ignoreMouseMotion;

    Camera cam;
} Context;

#endif