#include "autoly.h"
#include <X11/Xutil.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static XWindowAttributes attr;
static XButtonEvent pointer_origin;

void mouse_press(XButtonEvent *e) {
	// don't do anything if not clicking a window
	if (e->subwindow == None) {
		return;
	}

	// raise and focus window
	XRaiseWindow(dpy, e->subwindow);
	focus_window_by_id(e->subwindow);

	// if Mod1 not held, don't move/resize
	if (!(e->state & Mod1Mask)) {
		return;
	}

	// save pointer position for moving/resizing purposes
	XGetWindowAttributes(dpy, e->subwindow, &attr);
	pointer_origin = *e;

}

void mouse_release() {
	pointer_origin.subwindow = None;
}

// thanks tinywm!
void mouse_motion(XMotionEvent *e) {

	// if Mod1 not held, don't move/resize
	if (!(e->state & Mod1Mask)) {
		return;
	}

	// use previously saved pointer origin to determine what
	// the window's new size or position should be 
	if (pointer_origin.subwindow != None) {
		int xdiff = e->x_root - pointer_origin.x_root;
		int ydiff = e->y_root - pointer_origin.y_root;
		unsigned int new_width = MAX(1, attr.width + (pointer_origin.button==3 ? xdiff : 0));
		unsigned int new_height = MAX(1, attr.height + (pointer_origin.button==3 ? ydiff : 0));

		XSizeHints hints;
		long dummy;
		Status status = XGetWMNormalHints(dpy, pointer_origin.subwindow, &hints, &dummy);
		if (!status) return;

		// ensure the window cannot be resized smaller than the minimum allowed
		if (hints.min_width && (new_width < hints.min_width)) {
			new_width = hints.min_width;
		}

		if (hints.min_height && (new_height < hints.min_height)) {
			new_height = hints.min_height;
		}

		// ensure the window cannot be resized larger than the maximum allowed
		if (hints.max_height && (new_width > hints.max_width)) {
			new_width = hints.min_width;
		}

		if (hints.max_height && (new_height > hints.max_height)) {
			new_height = hints.min_height;
		}

		XMoveResizeWindow(dpy, pointer_origin.subwindow,
			attr.x + (pointer_origin.button==1 ? xdiff : 0),
			attr.y + (pointer_origin.button==1 ? ydiff : 0),
			new_width, new_height);
	}
}
