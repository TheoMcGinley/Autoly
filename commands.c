#include "autoly.h"
#include <string.h>

// cheap wayu of switching on string
#define CASE(c) if (!strcmp(e->data.b, c))

// sends argument supplied as a ClientMessage for the running instance
// of autoly to receive and handle in handle_message
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

Bool str_starts_with(const char *str, const char *pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

// processes messages sent from send_command
void handle_message(XClientMessageEvent *e) {
	// ICCCM requires iconify (hide)
	Atom wm_change_state = XInternAtom(dpy, "WM_CHANGE_STATE", False);
    if (e->message_type == wm_change_state &&
			e->format == 32 && e->data.l[0] == IconicState) {
		// TODO hide(e->window);
		return;
	}

	CASE("save") {
		printf("entering save mode...\n");
		save_mode();
	} else CASE("load") {
		printf("loading applications...\n");
	} else CASE("close") {
		destroy_focused_window();
	} else if (str_starts_with(e->data.b, "focus")) {
		int win_num = atoi(e->data.b+5);
		// if conversion successful, focus specified window
		if (win_num) {
			focus_window_by_number(win_num);
		}
	}
}
