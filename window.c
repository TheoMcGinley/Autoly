#include "CAbGC.h"

void windowMap(XMapEvent *e) {
	// if it's a new window, give it a border and associate 
	// the window with the current activity
	if (!existsInMap(e->window)) {
		giveBorder(e->window);
		addToMap(e->window, currentActivity);

		// TODO get wmclass of e->window
		// compare wmclass against all applications in currentActivity presets
		// if wmclass matches, move dimensions to fit preset
		/* struct Preset *p = &presets;
		while (p.next != NULL) {
			p = p->next;
		} */
		// TODO if new window fits the description of one of the 
		// applications in the current activity's preset
		// (e.g. _NET_WM_NAME == "Mozilla Firefox") 
		// then move and resize to preset
	}
	focusWindowByID(e->window);
}

void windowUnmap(XUnmapEvent *e) {
	printf("unmapping (doing nothing)\n");
}

void windowDestroy(XDestroyWindowEvent *e) {
	printf("destroying - removing from map\n");
	removeFromMap(e->window);
}

// All that is required by ICCCM is iconify (hide)
void windowMessage(XClientMessageEvent *e) {
	Atom wmChangeState = XInternAtom(dpy, "WM_CHANGE_STATE", False);
    if (e->message_type == wmChangeState && 
			e->format == 32 && e->data.l[0] == IconicState) {
		// TODO hide(e->window);
	}
}

void focusWindowByID(Window win) {
	// change the color of the old focused window back to non-focused
	XSetWindowBorder(dpy, getFocusedWindow(), NORMALBORDERCOLOR);

	XSetInputFocus(dpy, win, RevertToParent, CurrentTime);
	XSetWindowBorder(dpy, win, FOCUSBORDERCOLOR);
}

void focusWindowByNumber(int num) {
	// TODO XQueryTree, find nth window and focus it
}
