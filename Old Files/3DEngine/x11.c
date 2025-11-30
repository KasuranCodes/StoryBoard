/*
 * @brief Name: Max McGill
 * @brief Project: 3D Graphics Engine
 * @brief File: main.c
 * @brief Date: 08.07.25
 *
 */


#include<stdio.h>
#include<stdlib.h>

#include<X11/X.h>
#include<X11/Xlib.h>
#include <X11/keysym.h>  // For XK_* key symbols

#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

#include "src/main.c"
#include "controls.h"


/*---GLoBAL Declarables---*/
Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;




int main(int argc, char *argv[]) {

    dpy = XOpenDisplay(NULL);
    
    if(dpy == NULL) {
        printf("\n\tcannot connect to X server\n\n");
            exit(0);
    }

    root = DefaultRootWindow(dpy);
    vi = glXChooseVisual(dpy, 0, att);

    if (vi == NULL) {
        printf("\n\tno appropriate visual found\n\n");
            exit(0);
    } 
    else {
        printf("\n\tvisual %p selected\n", (void *)vi->visualid); /* %p creates hexadecimal output like in glxinfo */
    }

    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;

    win = XCreateWindow(dpy, root, 0, 0, 1280, 1280, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "VERY SIMPLE APPLICATION");
    
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Pixmap bm_no;
    XColor black;
    static char no_data[] = { 0,0,0,0,0,0,0,0 };
    Cursor no_ptr;

    bm_no = XCreateBitmapFromData(dpy, win, no_data, 8, 8);
    no_ptr = XCreatePixmapCursor(dpy, bm_no, bm_no, &black, &black, 0, 0);
    XDefineCursor(dpy, win, no_ptr);
    XFreeCursor(dpy, no_ptr);


    init();

    float moveSpeed = 0.1f;

    int ignoreNextMotion = False;


    while(1) {
        while (XPending(dpy)) {
            XNextEvent(dpy, &xev);


            if (xev.type == Expose) {
                XGetWindowAttributes(dpy, win, &gwa);
                glViewport(0, 0, gwa.width, gwa.height);
                windowHeight = gwa.height;
                windowWidth = gwa.width;
            }
            else if (xev.type == KeyPress) {
                KeySym keysym = XLookupKeysym(&xev.xkey, 0);
                printf("Key pressed: keycode=%u, keysym=%lu (%s)\n", xev.xkey.keycode, keysym, XKeysymToString(keysym));

                switch (keysym) {
                    case XK_Escape:
                        printf("Escape pressed\n");
                        // Cleanup and exit
                        glXMakeCurrent(dpy, None, NULL);
                        glXDestroyContext(dpy, glc);
                        XDestroyWindow(dpy, win);
                        XCloseDisplay(dpy);
                        exit(0);
                        break;

                    case XK_w:
                        printf("W key pressed\n");
                        // cam.position.z += 0.1;
                        keyPressed(&context, 'w');
                        break;

                    case XK_s:
                        printf("S key pressed\n");
                        // cam.position.z -= 0.1;
                        keyPressed(&context, 's');
                        break;

                    case XK_a:
                        printf("A key pressed\n");
                        // cam.position.x -= 0.1;
                        keyPressed(&context, 'a');
                        break;

                    case XK_d:
                        printf("D key pressed\n");
                        // cam.position.x += 0.1;
                        keyPressed(&context, 'd');
                        break;

                    case XK_space:
                        printf("Space key pressed\n");
                        keyPressed(&context, ' ');
                        break;

                    case XK_Control_L:
                        printf("Control key pressed\n");
                        keyPressed(&context, 'C');
                        break;

                    default:
                        printf("Other key: %lu\n", keysym);
                        break;
                }
            }
            else if (xev.type == MotionNotify) {
                mouseMoved(&context, xev, dpy, win, gwa);
            }
            else if (xev.type == ButtonPress) {
                switch (xev.xbutton.button) {
                    case Button1: printf("Left click\n"); break;
                    case Button2: printf("Middle click\n"); break;
                    case Button3: printf("Right click\n"); break;
                    case Button4: printf("Scroll up\n"); break;
                    case Button5: printf("Scroll down\n"); break;
                }
            }


        }

        // Render one frame
        frame();
        glXSwapBuffers(dpy, win);
    }

}
