#ifndef CONTROLS_H
#define CONTROLS_H

#include "vector.h"
#include "camera.h"
#include "context.h"


static void calcXYZ(float velocity, float direction, vec3 *out) {
    float angle = RADS(direction);
    out->x = velocity * cosf(angle);
    out->z = velocity * sinf(angle);
}

static void updatePosition(vec3 *currentPos, vec3 newPos) {
    *currentPos = vecAdd(*currentPos, newPos);
}

static void moveCam(Camera *cam, float direction) {
    vec3 newPosition = { 0 }; 
    calcXYZ(cam->velocity, direction, &newPosition);
    cam->position = vecAdd(cam->position, newPosition);
}

static void mouseMoved(Context *context, XEvent xev, Display *dpy, Window win, XWindowAttributes gwa) {
    if (context->ignoreMouseMotion) {
        context->ignoreMouseMotion = False;
        return;
    }

    int mouseX = xev.xmotion.x;
    int mouseY = xev.xmotion.y;

    int centerX = gwa.width / 2;
    int centerY = gwa.height / 2;

    int dx = mouseX - centerX;
    int dy = mouseY - centerY;

    // printf("Mouse dx: %d, dy: %d\n", dx, dy);
    float sensitivity = 0.1f;
    context->cam.direction.y += -(dx * sensitivity);  // yaw
    context->cam.direction.x += -(dy * sensitivity);  // pitch

    if (context->cam.direction.x > 89.0f) context->cam.direction.x = 89.0f;
    if (context->cam.direction.x < -89.0f) context->cam.direction.x = -89.0f;
    printf("Mouse dx: %f, dy: %f\n", context->cam.direction.x, context->cam.direction.y);

    // Warp back to center and skip the next motion event (triggered by the warp)
    XWarpPointer(dpy, None, win, 0, 0, 0, 0, centerX, centerY);
    context->ignoreMouseMotion = True;

}

static void keyPressed(Context *context, char key) {
    switch (context->menuState) {
        case 0:
        switch (key) {
            case 'w':
            context->cam.velocity = 1;
            moveCam(&context->cam, (context->cam.direction.y + 90 >= 360) ? context->cam.direction.y + 90 - 360 : context->cam.direction.y + 90);
            break;

            case 's':
            context->cam.velocity = 1;
            moveCam(&context->cam, (context->cam.direction.y + 270 >= 360) ? context->cam.direction.y + 270 - 360 : context->cam.direction.y + 270);
            break;

            case 'a':
            context->cam.velocity = 1;
            moveCam(&context->cam, (context->cam.direction.y + 180 >= 360) ? context->cam.direction.y + 180 - 360 : context->cam.direction.y + 180);
            break;
            
            case 'd':
            context->cam.velocity = 1;
            moveCam(&context->cam, (context->cam.direction.y + 0 >= 360) ? context->cam.direction.y + 0 - 360 : context->cam.direction.y + 0);
            break;

            case ' ':
            cam.position.y += 1;
            break;

            case 'C':
            cam.position.y -= 1;
            break;

        }
        break;

    }
}


#endif
