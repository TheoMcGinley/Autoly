#include "CAbGC.h" 
#include <X11/Xmd.h>

void executeCommand(const char *command) {
	if (fork() == 0) {
		execl(_PATH_BSHELL, _PATH_BSHELL, "-c", command, NULL);
	}
}

// determines whether the window provides the passed atom as available information
Bool windowProvidesAtom(Window win, Atom a) {

	// iterate through window properties to determine if atom exists
	int nProperties;
	Bool providesAtom = False;
	Atom *windowProperties = XListProperties(dpy, win, &nProperties);
	for (int i=0; i<nProperties; i++) {
		if (windowProperties[i] == a) {
			providesAtom = True;
		}
	}
	XFree(windowProperties);

	return providesAtom;
}

// giveBorder gives all regular windows a border, but doesn't
// give a border to dropdown menus, dialogue boxes etc.
void giveBorder(Window win) {

	// if the client does not specify its window type, give
	// it a border by default
	Atom windowType = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", True);
	if (!windowProvidesAtom(win, windowType)) {
		XSetWindowBorder(dpy, win, FOCUSBORDERCOLOR);
		XSetWindowBorderWidth(dpy, win, BORDERTHICKNESS);
		return;
	}

	Atom actualType;
	int actualFormat, status;
	unsigned long nItems, bytesAfter;
	unsigned char *propReturn = NULL;
	// adapted from xprop source code
	status = XGetWindowProperty(dpy, win, windowType,
			0L, sizeof(Atom), False, AnyPropertyType,
			&actualType, &actualFormat, &nItems, &bytesAfter,
			&propReturn);

	// if the query fails, do not give the window a border
	if (status != Success) {
		XFree(propReturn);
		return;
	}

	// clients may specify multiple window types - if any of them specify
	// that the window is a normal window, then give the window a border
	Atom prop;
	Atom NormalWindow = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NORMAL", True);
	for (int i=0; i<nItems; i++) {
		prop = ((Atom *)propReturn)[i];
		if (prop == NormalWindow) {
			XSetWindowBorder(dpy, win, FOCUSBORDERCOLOR);
			XSetWindowBorderWidth(dpy, win, BORDERTHICKNESS);
		}
	}

	XFree(propReturn);
	return;

}

Window getFocusedWindow() {
	printf("entered getFocused \n");
	Window focused = None;
	int revert;
	printf("getting focused \n");
	XGetInputFocus(dpy, &focused, &revert);
	printf("got focused, returning \n");
	return focused;
}

// if client acknowledges ICCCM's WM_DELETE_WINDOW, close it nicely, else KILL
// Calling this will in turn call an unmap and/or window destruction, removing
// it from the grid
void destroyFocusedWindow() {
	printf("entered destroyFocusedWindow\n");
	Window focused = getFocusedWindow();
	printf("got focused\n");
	printf("got focused, focused = %lx\n", focused);
	int revert;
	XGetInputFocus(dpy, &focused, &revert);

	//please don't try and kill the root window
	if (focused == DefaultRootWindow(dpy)) return;

	printf("passed default \n");
	
    int i, n, found = 0;
    Atom *protocols;
    Atom wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", False);

	printf("getting protocols \n");
    if (XGetWMProtocols(dpy, focused, &protocols, &n)) {
        for (i=0; i<n; i++) if (protocols[i] == wmDelete) found++;
        XFree(protocols);
    }

	printf("got protocols \n");
    if (found) {
		printf("found\n");
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
		printf("KILLING\n");
		XKillClient(dpy, focused);
	}
}
