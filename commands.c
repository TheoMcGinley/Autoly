#include "CAbGC.h"
#include <string.h>

#define CASE(c) if (!strcmp(e->data.b, c))

void send_command(int argc, char **argv) {
	if (!(dpy = XOpenDisplay(0x0))) {
		return;
	}

	XEvent ev;
	ev.xclient.type = ClientMessage;
	ev.xclient.window = RootWindow(dpy, DefaultScreen(dpy));
	ev.xclient.message_type = XInternAtom(dpy, "WM_CHANGE_STATE", True);
	ev.xclient.format = 8;
	memcpy(ev.xclient.data.b, argv[1], 20);

	XSendEvent(dpy, RootWindow(dpy, DefaultScreen(dpy)), False, SubstructureRedirectMask|SubstructureNotifyMask, &ev);
	XFlush(dpy);
}


// handle messages sent to the wm for closing, focusing windows etc
// running the command "cabgc close" will end up sending an event here
// with e->data.b equal to "close"
void handleMessage(XClientMessageEvent *e) {
	// ICCCM requires iconify (hide)
	Atom wmChangeState = XInternAtom(dpy, "WM_CHANGE_STATE", False);
    if (e->message_type == wmChangeState &&
			e->format == 32 && e->data.l[0] == IconicState) {
		// TODO hide(e->window);
		return;
	}
	
	CASE("save") {
		printf("entering save mode...\n");
		saveMode();
	} else CASE("load") {
		printf("loading applications...\n");
	} else CASE("close") {
		destroyFocusedWindow();
	}

}
