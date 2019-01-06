#include "autoly.h" 
#include <X11/Xmd.h>
#include <X11/Xutil.h>
char *get_wm_class(Window win) {
	// hint.res_name = application name
	// hint.res_class = application class
	XClassHint hint;
	int status = XGetClassHint(dpy, win, &hint);

	if (!status) {
		XFree(hint.res_name);
		XFree(hint.res_class);
		return NULL;
	}

	XFree(hint.res_name);
	return hint.res_class;
}

void execute_command(const char *command) {
	if (fork() == 0) {
		execl(_PATH_BSHELL, _PATH_BSHELL, "-c", command, NULL);
	}
}

// determines whether the window provides the passed atom as available information
Bool window_provides_atom(Window win, Atom a) {

	// iterate through window properties to determine if atom exists
	int num_properties;
	Bool provides_atom = False;
	Atom *window_properties = XListProperties(dpy, win, &num_properties);
	for (int i=0; i<num_properties; i++) {
		if (window_properties[i] == a) {
			provides_atom = True;
		}
	}
	XFree(window_properties);

	return provides_atom;
}

// give_border gives all regular windows a border, but doesn't
// give a border to dropdown menus, dialogue boxes etc.
void give_border(Window win) {

	// if the client does not specify its window type, give
	// it a border by default
	Atom window_type = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", True);
	if (!window_provides_atom(win, window_type)) {
		XSetWindowBorder(dpy, win, FOCUSBORDERCOLOR);
		XSetWindowBorderWidth(dpy, win, BORDERTHICKNESS);
		return;
	}

	Atom actual_type;
	int actual_format, status;
	unsigned long num_items, bytes_after;
	unsigned char *prop_return = NULL;
	// adapted from xprop source code
	status = XGetWindowProperty(dpy, win, window_type,
			0L, sizeof(Atom), False, AnyPropertyType,
			&actual_type, &actual_format, &num_items, &bytes_after,
			&prop_return);

	// if the query fails, do not give the window a border
	if (status != Success) {
		XFree(prop_return);
		return;
	}

	// clients may specify multiple window types - if any of them specify
	// that the window is a normal window, then give the window a border
	Atom prop;
	Atom normal_window = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NORMAL", True);
	for (int i=0; i<num_items; i++) {
		prop = ((Atom *)prop_return)[i];
		if (prop == normal_window) {
			XSetWindowBorder(dpy, win, FOCUSBORDERCOLOR);
			XSetWindowBorderWidth(dpy, win, BORDERTHICKNESS);
		}
	}

	XFree(prop_return);
	return;

}

Window get_focused_window() {
	Window focused = None;
	int revert;
	XGetInputFocus(dpy, &focused, &revert);
	return focused;
}

// if client acknowledges ICCCM's WM_DELETE_WINDOW, close it nicely
// else KILL
void destroy_focused_window() {
	Window focused = get_focused_window();

	//please don't try and kill the root window
	if (focused == DefaultRootWindow(dpy)) return;

    int i, n, found = 0;
    Atom *protocols;
    Atom wm_delete = XInternAtom(dpy, "WM_DELETE_WINDOW", False);

    if (XGetWMProtocols(dpy, focused, &protocols, &n)) {
        for (i=0; i<n; i++) if (protocols[i] == wm_delete) found++;
        XFree(protocols);
    }

    if (found) {
		//from: https://nachtimwald.com/2009/11/08/sending-wm_delete_window-client-messages/
		XEvent ev;
		ev.xclient.type = ClientMessage;
		ev.xclient.window = focused;
		ev.xclient.message_type = XInternAtom(dpy, "WM_PROTOCOLS", True);
		ev.xclient.format = 32;
		ev.xclient.data.l[0] = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
		ev.xclient.data.l[1] = CurrentTime;

		XSendEvent(dpy, focused, False, NoEventMask, &ev);	
	} else {
		XKillClient(dpy, focused);
	}
}
