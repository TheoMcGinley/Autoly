#ifndef CABGC_H
#define CABGC_H

// INCLUDES {{{
#include <X11/Xlib.h>
#include <stdio.h>     // used for: printf
#include <unistd.h>    // used for: NULL, exit, fork, sleep
#include <stdlib.h>    // used for: NULL, malloc, free, exit, system
#include <paths.h>     // used for: ???
#include <string.h>    // used for strcmp, strcpy, ...
#include <fcntl.h>     // used for: open
#include <sys/types.h> // used for: mkfifo
#include <sys/stat.h>  // used for: mkfio
// END_INCLUDES }}}

// DEFINES {{{

#define PIPE_FILE "/tmp/cabgc"

#define NORMALBORDERCOLOR 2367780
#define FOCUSBORDERCOLOR 16777215

#define NUMACTIVITIES 20
#define BORDERTHICKNESS 10

// definitions for hiding - can't find where to import them from
#define WithdrawnState 0
#define NormalState 1
#define IconicState 3

// END_DEFINES }}}

// STRUCTS {{{

struct Application {
	char *wm_class;
	int width;
	int height;
	int x;
	int y;
	struct Application *next;
};

struct Preset {
	char *hotkey;
	struct Application applicationList;
	struct Preset *next;
};

struct Keybind {
	char *hotkey;
	char *command;
	struct Keybind *next;
};

// END_STRUCTS }}}

// GLOBALS {{{

enum wmMode {NORMAL, SAVE};

extern Display * dpy;
extern char currentActivity[10];
extern enum wmMode wmMode;
extern struct Preset presets;
extern struct Keybind keybinds;

// END_GLOBALS }}}

// FUNCTIONS {{{

// commands.c
extern void listen_for_commands();
extern void send_command(int, char **);
extern void execute_wm_command(char *);

// config.c
extern void loadConfig();

// key.c
extern void keyPress(XKeyEvent *);

// map.c
extern void addToMap(Window, char *);
extern void removeFromMap(Window);
extern char *getActivityID(Window);
extern int  existsInMap(Window);
extern void freeMap();
extern void switchToActivity(char *);
extern void moveFocusedToActivity(char *);


// mouse.c
extern void mousePress(XButtonEvent *);
extern void mouseRelease();
extern void mouseMotion(XMotionEvent *);

// preset.c
extern void saveMode();
extern void read_presets();
extern void write_preset(const char *);

// utils.c 
extern void executeCommand(const char *);
extern void giveBorder(Window);
extern void destroyFocusedWindow();
extern Window getFocusedWindow();
extern Bool windowProvidesAtom(Window, Atom);

// window.c
extern void windowMap(XMapEvent *);
extern void windowUnmap(XUnmapEvent *);
extern void windowDestroy(XDestroyWindowEvent *);
extern void windowMessage(XClientMessageEvent *);
extern void focusWindowByID(Window);
extern void focusWindowByNumber(int);

// END_FUNCTIONS }}}
#endif // CABGC_H
