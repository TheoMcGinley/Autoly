#include "autoly.h"

void window_map(XMapEvent *e) {
	// if it's a new window, give it a border and associate 
	// the window with the current workspace
	if (!exists_in_map(e->window)) {
		give_border(e->window);
		add_to_map(e->window, current_workspace);

		// if the WM_CLASS of the new window matches any in the layout,
		// change the geometry of the window to fit the layout
		char *class = get_wm_class(e->window);
		Layout *l = &layouts;
		while (l.next != NULL) {
			l = l->next;
		}

		XFree(class);
		// TODO if new window fits the description of one of the 
		// applications in the current workspace's preset
		// (e.g. _NET_WM_NAME == "Mozilla Firefox") 
		// then move and resize to preset
	}
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
	// TODO XQueryTree, find nth window and focus it
}
