#include "autoly.h"

// determine if the given position already has a window in,
// not accounting for the new window that has just been created
Bool window_position_occupied(int x, int y, Window win_to_ignore) {
	// assume position not occupied
	Bool is_occupied = False;

	// get all mapped windows
	Window dummy1, dummy2, *window_list;
	unsigned int num_windows;
	XQueryTree(dpy, DefaultRootWindow(dpy), &dummy1, &dummy2, &window_list, &num_windows);

	printf("windows found: %d\n", num_windows);
	// check if any of the windows occupy the specified position
	for (int i=0; i<num_windows; i++) {
		// ignore the new window
		if (window_list[i] == win_to_ignore) {
			printf("ignoring new window, continuing...\n");
			continue;
		}

		int win_x = -1, win_y = -1;
		unsigned int width, height, dummy3;
		int status = XGetGeometry(dpy, window_list[i], &dummy1, &win_x, &win_y, &width, &height, &dummy3, &dummy3);

		printf("window %d x and y: %d, %d\n", i, win_x, win_y);
		if (!status) goto cleanup;

		if (win_x == x && win_y == y) {
			printf("it's occupied!\n");
			is_occupied	= True;
			break;
		}
	}

cleanup:
	XFree(window_list);
	return is_occupied;
}

void window_map(XMapEvent *e) {
	// if it's a new window, give it a border and associate 
	// the window with the current workspace
	if (!exists_in_map(e->window)) {
		give_border(e->window);
		add_to_map(e->window, current_layout->hotkey);

		// if the WM_CLASS of the new window matches any in the layout,
		// change the geometry of the window to fit the layout
		char *class = get_wm_class(e->window);
		Application *apps = &current_layout->app_list;

		while (apps->next != NULL) {
			apps = apps->next;

			// if the new window's WM_CLASS matches an application in the layout
			// and the application does not already exist
			if (!strcmp(apps->wm_class, class) &&
			!window_position_occupied(apps->x, apps->y, e->window)) {
				XMoveResizeWindow(dpy, e->window,
				apps->x, apps->y, apps->width, apps->height);
				break;
			}
		}

		XFree(class);
	}

	// focus new window
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
	printf("focusing window %d\n", num);
	// TODO XQueryTree, find nth window and focus it
}
