#include "autoly.h"

// determine if the given position already has a window in,
// not accounting for the new window that has just been created
Bool window_position_occupied(int x, int y, Window win_to_ignore) {
	debug_log("window.c: window_position_occupied: checking...\n");
	// assume position not occupied
	Bool is_occupied = False;

	// get all mapped windows
	Window dummy1, dummy2, *window_list;
	unsigned int num_windows;
	XQueryTree(dpy, DefaultRootWindow(dpy), &dummy1, &dummy2, &window_list, &num_windows);

	// check if any of the windows occupy the specified position
	for (int i=0; i<num_windows; i++) {
		// ignore the new window
		if (window_list[i] == win_to_ignore) {
			continue;
		}

		XWindowAttributes attr;
		int status = XGetWindowAttributes(dpy, window_list[i], &attr);
		if (!status) goto cleanup;

		// ignore non-mapped windows
		if (attr.map_state != IsViewable) {
			continue;
		}

		if (!status) goto cleanup;

		if (attr.x == x && attr.y == y) {
			debug_log("window.c: window_position_occupied: position occupied\n");
			is_occupied	= True;
			break;
		}
	}

cleanup:
	if (window_list != NULL) {
		debug_log("window.c: window_position_occupied: freeing window list\n");
		XFree(window_list);
	}
	return is_occupied;
}

void window_map(XMapEvent *e) {
	debug_log("window.c: window_map: mapping window\n");
	// if it's a new window, give it a border and associate 
	// the window with the current workspace
	if (!exists_in_map(e->window)) {
		give_border(e->window);
		add_to_map(e->window, current_layout->hotkey);

		// if the WM_CLASS of the new window matches any in the layout,
		// change the geometry of the window to fit the layout
		char *class = get_wm_class(e->window);
		Application *apps = current_layout->app_list;
		Bool window_moved = False;
		while (apps->next != NULL) {
			apps = apps->next;

			// if the new window's WM_CLASS matches an application in the layout
			// and the application does not already exist
			if (!strcmp(apps->wm_class, class) &&
			!window_position_occupied(apps->x, apps->y, e->window)) {
				XMoveResizeWindow(dpy, e->window,
				apps->x, apps->y, apps->width, apps->height);
				window_moved = True;
				break;
			}
		}

		if (class != NULL) {
			XFree(class);
		}

		// if window has not been moved by the layout, move it to the centre
		if (!window_moved) {
			XWindowAttributes attr;
			if (XGetWindowAttributes(dpy, e->window, &attr)) {
				XMoveWindow(dpy, e->window, XDisplayWidth(dpy, DefaultScreen(dpy))/2 - attr.width,
				XDisplayHeight(dpy, DefaultScreen(dpy))/2 - attr.height);
			}
		}
	}

	// focus new window
	debug_log("window.c: window_map: focused window %lx\n", e->window);
	focus_window_by_id(e->window);
}

void window_unmap(XUnmapEvent *e) {

}

void window_destroy(XDestroyWindowEvent *e) {
	remove_from_map(e->window);
}

void focus_window_by_id(Window win) {
	// change the color of the old focused window back to non-focused
	XSetWindowBorder(dpy, get_focused_window(), NORMALBORDERCOLOR);

	XSetInputFocus(dpy, win, RevertToParent, CurrentTime);
	XSetWindowBorder(dpy, win, FOCUSBORDERCOLOR);
}

void focus_window_by_number(int num) {
	debug_log("window.c: focus_window_by_number: pretending to focus window %d\n", num);
	// TODO XQueryTree, find nth window and focus it
}
