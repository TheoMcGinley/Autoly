#include "autoly.h"
#define GRAB_KEY(K)       XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym(K)), Mod1Mask, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync)
#define GRAB_SHIFT_KEY(K) XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym(K)), ShiftMask|Mod1Mask, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync)
#define GRAB_MOUSE_KEY(K) XGrabButton(dpy, K, Mod1Mask, DefaultRootWindow(dpy), True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None)

Display * dpy;
char current_workspace[10];
enum WMmode wm_mode;
Layout layouts;
Keybind keybinds;

// fakeErrorHandler ensures that the wm does not halt on any errors
int fake_error_handler(Display *d, XErrorEvent *e) {
	char msg[80];

	XGetErrorText(d, e->error_code, msg, sizeof(msg));

	fprintf(stderr, "Error %d (%s): request %d.%d\n",
					e->error_code, msg, e->request_code,
					e->minor_code);
	return 0;
}

// not pretty but functional - saves having to redirect 
// XSelectInput wheneiver a client grabs the keyboard
// DWM takes the same approach
static void grab_peripherals() {
	// grab left and right click
	GRAB_MOUSE_KEY(1);
	GRAB_MOUSE_KEY(3);

	// grab necessary keyboard keys
	GRAB_KEY("a");
	GRAB_KEY("b");
	GRAB_KEY("c");
	GRAB_KEY("d");
	GRAB_KEY("e");
	GRAB_KEY("f");
	GRAB_KEY("g");
	GRAB_KEY("h");
	GRAB_KEY("i");
	GRAB_KEY("j");
	GRAB_KEY("k");
	GRAB_KEY("l");
	GRAB_KEY("m");
	GRAB_KEY("n");
	GRAB_KEY("o");
	GRAB_KEY("p");
	GRAB_KEY("q");
	GRAB_KEY("r");
	GRAB_KEY("s");
	GRAB_KEY("t");
	GRAB_KEY("u");
	GRAB_KEY("v");
	GRAB_KEY("w");
	GRAB_KEY("x");
	GRAB_KEY("y");
	GRAB_KEY("z");

	GRAB_KEY("1");
	GRAB_KEY("2");
	GRAB_KEY("3");
	GRAB_KEY("4");
	GRAB_KEY("5");
	GRAB_KEY("6");
	GRAB_KEY("7");
	GRAB_KEY("8");
	GRAB_KEY("9");
	GRAB_KEY("0");

	GRAB_KEY("F1");
	GRAB_KEY("F2");
	GRAB_KEY("F3");
	GRAB_KEY("F4");
	GRAB_KEY("F5");
	GRAB_KEY("F6");
	GRAB_KEY("F7");
	GRAB_KEY("F8");
	GRAB_KEY("F9");
	GRAB_KEY("F10");
	GRAB_KEY("F11");
	GRAB_KEY("F12");

	GRAB_KEY("semicolon");
	GRAB_KEY("comma");
	GRAB_KEY("period");
	GRAB_KEY("slash");
	GRAB_KEY("apostrophe");

	GRAB_KEY("Return"); 
	GRAB_KEY("Escape"); 
	GRAB_KEY("BackSpace"); 
	GRAB_KEY("Tab");
	GRAB_KEY("Insert"); 
	GRAB_KEY("Delete"); 
	GRAB_KEY("Prior"); // PG UP
	GRAB_KEY("Next");  // PG DOWN
	GRAB_KEY("space");
}

static int init() {

    if(!(dpy = XOpenDisplay(0x0))) {
		return 0;
	}

	wm_mode = NORMAL;

	// make all children of root give out notify events
	XSelectInput(dpy, RootWindow(dpy, DefaultScreen(dpy)), SubstructureNotifyMask);
	XSetInputFocus(dpy, RootWindow(dpy, DefaultScreen(dpy)), RevertToPointerRoot, CurrentTime);

	grab_peripherals();
	load_layouts();
	load_config();

	// use first layout as start-up workspace
	strcpy(current_workspace, layouts.next->hotkey);

	// ensure mouse is ready to move windows
	mouse_release();

	// XSetErrorHandler(fakeErrorHandler);
	return 1;
}

int main(int argc, char **argv) {

	// if program has arguments, interpret the arguments as a command
	if (argc > 1) {
		send_command(argc, argv);
		return 0;
	}

	// if initialisation failed, exit with error
	if(!init()) {
		return 1;	
	}

	// handle all incoming XEvents
	XEvent ev;
    for(;;) {

		// process next XEvent (blocks until event found)
        XNextEvent(dpy, &ev);
		switch (ev.type) {
            case ButtonPress:   mouse_press(&ev.xbutton);           break;
            case ButtonRelease: mouse_release();                    break;
			case MotionNotify:  mouse_motion(&ev.xmotion);          break;
			case KeyPress:      key_press(&ev.xkey);                break;
			case MapNotify:     window_map(&ev.xmap);               break;
            case UnmapNotify:   window_unmap(&ev.xunmap);           break;
			case DestroyNotify: window_destroy(&ev.xdestroywindow); break;
            case ClientMessage: handle_message(&ev.xclient);        break;
		}

    }

	return 0;
}
