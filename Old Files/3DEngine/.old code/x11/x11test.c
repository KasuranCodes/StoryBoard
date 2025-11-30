
/*
 * @brief Name: Max McGill
 * @brief Project: Test Window
 * @brief Date: 14.05.25
 *
 */

/*---Includes---*/
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>




#include <stdio.h>
#include <stdlib.h>


/*---Definitions---*/
#define WINDOWXPOS 		50
#define WINDOWYPOS 		50
#define WINDOWWIDTH 		1280
#define WINDOWHEIGHT 		720
#define WINDOWTITLE 		"Test Window"
#define BORDER 			1

/*---Global Declarables---*/

static Display* display;
static int screen;
static Window root;


/*
 * @brief MainFunction
 * @param void
 * @retval int - StatusCode
 */
int main()
{
	Window w;
	XEvent ev;

	if ((display = XOpenDisplay(NULL)) == NULL)	{fprintf(stderr, "Can't Open Display");}

	screen = DefaultScreen(display);
	root = RootWindow(display, screen);	
	
	w = XCreateSimpleWindow(display, root, WINDOWXPOS, WINDOWYPOS, WINDOWWIDTH, WINDOWHEIGHT, BORDER, BlackPixel(display, screen), WhitePixel(display, screen));
	XMapWindow(display, w);
	
    XStoreName(display, w, WINDOWTITLE);
    eglBindAPI(EGL_OPENGL_ES3_BIT);

	while (XNextEvent(display, &ev) == 0)
	{

	}

	XUnmapWindow(display, w);

	XDestroyWindow(display, w);
	
	XCloseDisplay(display);

	return 0;
}