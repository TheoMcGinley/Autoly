#include "CAbGC.h"
#define GRAB_KEY(K)       XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym(K)), Mod1Mask, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync)
#define GRAB_SHIFT_KEY(K) XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym(K)), ShiftMask|Mod1Mask, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync)
#define GRAB_MOUSE_KEY(K) XGrabButton(dpy, K, Mod1Mask, DefaultRootWindow(dpy), True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None)

#include <sys/types.h> // mkfifo
#include <sys/stat.h> // mkfifo

Display * dpy;
char currentActivity[10];
enum wmMode wmMode;
struct Preset presets;
struct Keybind keybinds;

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
	struct Preset *REMOVEME = presets.next;
	printf("HOTKEY1: %s\n", REMOVEME->hotkey);
	printf("HOTKEY APPLICATION1 CLASS: %s\n", REMOVEME->applicationList.next->wm_class);
	printf("HOTKEY APPLICATION2 CLASS: %s\n", REMOVEME->applicationList.next->next->wm_class);
	REMOVEME = REMOVEME->next;
	printf("HOTKEY2: %s\n", REMOVEME->hotkey);
	printf("HOTKEY2 APPLICATION1 CLASS: %s\n", REMOVEME->applicationList.next->wm_class);
	printf("HOTKEY2 APPLICATION2 CLASS: %s\n", REMOVEME->applicationList.next->next->wm_class);

	// TODO this requires at least one preset defined on startup
	// figure out a better solution
	// move to the first defined preset
	// strcpy(currentActivity, presets.hotkey);

	// ensure mouse is ready to move windows
	mouseRelease();

	XSetErrorHandler(fakeErrorHandler);
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

	// handle all incoming XEvents and messages to cabgc
	char *pipe = PIPE_FILE;
	mkfifo(pipe, 0666);
	int pipe_fd = open(pipe, O_RDONLY | O_NONBLOCK);
	XEvent ev;
    for(;;) {

		// process next XEvent
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
		}

		// process message to wm (thanks hootwm!)
		char buffer[511];
		int length;
        if (length = read(pipe_fd, buffer, sizeof(buffer))) {
            buffer[length-1] = '\0';
            execute_wm_command(buffer);
		}
    }

	return 0;
}


