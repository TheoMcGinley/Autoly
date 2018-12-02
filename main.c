#include "CAbGC.h"
#define GRAB_KEY(K)       XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym(K)), Mod1Mask, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync)
#define GRAB_SHIFT_KEY(K) XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym(K)), ShiftMask|Mod1Mask, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync)
#define GRAB_MOUSE_KEY(K) XGrabButton(dpy, K, Mod1Mask, DefaultRootWindow(dpy), True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None)

Display * dpy;
int currentActivity;
enum wmMode wmMode;
struct Preset presets;

// fakeErrorHandler ensures that the wm does not halt on any errors
int fakeErrorHandler(Display *d, XErrorEvent *e) {
	char msg[80];

	XGetErrorText(d, e->error_code, msg, sizeof(msg));

	fprintf(stderr, "Error %d (%s): request %d.%d\n",
					e->error_code, msg, e->request_code,
					e->minor_code);
	return 0;
}

static int init() {

    if(!(dpy = XOpenDisplay(0x0))) {
		return 0;
	}

	currentActivity = 0;
	wmMode = NORMAL;

	// make all children of root give out notify events
	XSelectInput (dpy, RootWindow(dpy, DefaultScreen(dpy)), SubstructureNotifyMask);
	XSetInputFocus(dpy, RootWindow(dpy, DefaultScreen(dpy)), RevertToPointerRoot, CurrentTime);

	// grab left and right click
	GRAB_MOUSE_KEY(1);
	GRAB_MOUSE_KEY(3);

	// grab necessary keyboard keys
	GRAB_KEY("Return"); 
	GRAB_KEY("s"); // save layout
	GRAB_KEY("l"); // load applications in layout
	GRAB_KEY("d"); // development activity
	GRAB_KEY("g"); // games activity
	GRAB_KEY("b"); // browsing activity
	GRAB_KEY("q"); // close focused window
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

	GRAB_SHIFT_KEY("1");
	GRAB_SHIFT_KEY("2");

	loadPresets();

	// ensure mouse is ready to move windows
	mouseRelease();

	XSetErrorHandler(fakeErrorHandler);
	return 1;
}

int main(void) {

	//if initialisation failed, exit with error
	if(!init()) {
		return 1;	
	}

	// handle all incoming XEvents
	XEvent ev;
    for(;;) {

        XNextEvent(dpy, &ev);

		switch (ev.type) {
            case ButtonPress:   mousePress(&ev.xbutton);           break;
            case ButtonRelease: mouseRelease();                    break;
			case MotionNotify:  mouseMotion(&ev.xmotion);          break;
			case KeyPress:      keyPress(&ev.xkey);                break;
			case MapNotify:     windowMap(&ev.xmap);               break;
            case UnmapNotify:   windowUnmap(&ev.xunmap);           break;
			case DestroyNotify: windowDestroy(&ev.xdestroywindow); break;
            case ClientMessage: windowMessage(&ev.xclient);        break;
            // case ConfigureRequest:
            // case Expose:
		}

    }

	return 0;
}


