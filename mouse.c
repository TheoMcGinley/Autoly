#include "CAbGC.h"
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static XWindowAttributes attr;
static XButtonEvent pointerOrigin;

// TODO resetFocusedBorder -> focus e->window?
void mousePress(XButtonEvent *e) {
	// don't do anything if clicking on the background
	if (e->subwindow == None) {
		return;
	}

	// save pointer position for moving/resizing purposes
	XGetWindowAttributes(dpy, e->subwindow, &attr);
	pointerOrigin = *e;

	XRaiseWindow(dpy, e->subwindow);
	// focus this window
	focusWindowByID(e->subwindow);
}

void mouseRelease() {
	pointerOrigin.subwindow = None;
}

// thanks tinywm!
void mouseMotion(XMotionEvent *e) {
	// compress motion notify events down to most recent
	// while(XCheckTypedEvent(dpy, MotionNotify, &e));

	// use previously saved pointer origin to determine what
	// the window's new size or position should be 
	if (pointerOrigin.subwindow != None) {
		int xdiff = e->x_root - pointerOrigin.x_root;
		int ydiff = e->y_root - pointerOrigin.y_root;
		XMoveResizeWindow(dpy, pointerOrigin.subwindow,
			attr.x + (pointerOrigin.button==1 ? xdiff : 0),
			attr.y + (pointerOrigin.button==1 ? ydiff : 0),
			MAX(1, attr.width + (pointerOrigin.button==3 ? xdiff : 0)),
			MAX(1, attr.height + (pointerOrigin.button==3 ? ydiff : 0)));
	}
}

