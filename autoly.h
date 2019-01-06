#ifndef AUTOLY_H
#define AUTOLY_H

// INCLUDES {{{
#include <X11/Xlib.h>
#include <stdio.h>     // used for: printf
#include <unistd.h>    // used for: NULL, exit, fork, sleep
#include <stdlib.h>    // used for: NULL, malloc, free, exit, system
#include <paths.h>     // used for: ???
#include <string.h>    // used for strcmp, strcpy, ...
#include <fcntl.h>     // used for: open
#include <sys/types.h> // used for: mkfifo
#include <sys/stat.h>  // used for: mkfifo
// END_INCLUDES }}}

// DEFINES {{{

// border settings
#define NORMALBORDERCOLOR 2367780
#define FOCUSBORDERCOLOR 16777215
#define BORDERTHICKNESS 10

// definitions for hiding - can't find where to import them from
#define WithdrawnState 0
#define NormalState 1
#define IconicState 3

// END_DEFINES }}}

// STRUCTS {{{

typedef struct _Application {
	char *wm_class;
	int width;
	int height;
	int x;
	int y;
	struct _Application *next;
} Application;

typedef struct _Layout {
	char *hotkey;
	Application app_list;
	struct _Layout *next;
} Layout;

typedef struct _Keybind {
	char *hotkey;
	char *command;
	struct _Keybind *next;
} Keybind;

// END_STRUCTS }}}

// GLOBALS {{{

enum WMmode {NORMAL, SAVE};

extern Display * dpy;
extern char current_workspace[10];
extern enum WMmode wm_mode;
extern Layout layouts;
extern Keybind keybinds;

// END_GLOBALS }}}

// FUNCTIONS {{{

// commands.c
extern void send_command(int, char **);
extern void handle_message(XClientMessageEvent *);

// config.c
extern void load_config();

// key.c
extern void key_press(XKeyEvent *);

// map.c
extern void add_to_map(Window, char *);
extern void remove_from_map(Window);
extern char *get_workspace_id(Window);
extern int  exists_in_map(Window);
extern void free_map();
extern void switch_to_workspace(char *);
extern void move_focused_to_workspace(char *);


// mouse.c
extern void mouse_press(XButtonEvent *);
extern void mouse_release();
extern void mouse_motion(XMotionEvent *);

// layout.c
extern void save_mode();
extern void load_layouts();
extern void save_layout(const char *);
extern char *get_wm_class(Window);

// utils.c 
extern void   execute_command(const char *);
extern void   give_border(Window);
extern void   destroy_focused_window();
extern Window get_focused_window();
extern Bool   window_provides_atom(Window, Atom);

// window.c
extern void window_map(XMapEvent *);
extern void window_unmap(XUnmapEvent *);
extern void window_destroy(XDestroyWindowEvent *);
extern void focus_window_by_id(Window);
extern void focus_window_by_number(int);

// END_FUNCTIONS }}}
#endif
