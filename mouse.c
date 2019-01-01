#include "autoly.h"
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static XWindowAttributes attr;
static XButtonEvent pointer_origin;

void mouse_press(XButtonEvent *e) {
	// don't do anything if not clicking a window
	if (e->subwindow == None) {
		return;
	}

	// save pointer position for moving/resizing purposes
	XGetWindowAttributes(dpy, e->subwindow, &attr);
	pointer_origin = *e;

	// raise and focus window
	XRaiseWindow(dpy, e->subwindow);
	focus_window_by_id(e->subwindow);
}

void mouse_release() {
	pointer_origin.subwindow = None;
}

// thanks tinywm!
void mouse_motion(XMotionEvent *e) {
	// compress motion notify events down to most recent
	// while(XCheckTypedEvent(dpy, MotionNotify, &e));

	// use previously saved pointer origin to determine what
	// the window's new size or position should be 
	if (pointer_origin.subwindow != None) {
		int xdiff = e->x_root - pointer_origin.x_root;
		int ydiff = e->y_root - pointer_origin.y_root;
		XMoveResizeWindow(dpy, pointer_origin.subwindow,
			attr.x + (pointer_origin.button==1 ? xdiff : 0),
			attr.y + (pointer_origin.button==1 ? ydiff : 0),
			MAX(1, attr.width + (pointer_origin.button==3 ? xdiff : 0)),
			MAX(1, attr.height + (pointer_origin.button==3 ? ydiff : 0)));
	}
}
