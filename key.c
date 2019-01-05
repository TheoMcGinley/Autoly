#include "autoly.h"
#include <X11/XKBlib.h> // needed for XkbKeycodeToKeysym

// if the given key is mapped to a command, execute the command
int config_command(char *key_name) {
	// iterate through keybinds until hotkey found or end of list
	Keybind *kb = &keybinds;
	while (kb->next != NULL) {
		kb = kb->next;
		if (!strcmp(kb->hotkey, key_name)) {
			execute_command(kb->command);
			return 1;
		}
	}
	return 0;
}


// iterate through layouts until hotkey found or end of list
int is_workspace(char *key_name) {
	Layout *l = &layouts;
	while (l->next != NULL) {
		l = l->next;
		if (!strcmp(l->hotkey, key_name)) {
			return 1;
		}
	}
	return 0;
}

// handle key presses when MOD held
void key_press(XKeyEvent *e) {
	char *key_name = XKeysymToString(XkbKeycodeToKeysym(dpy, e->keycode, 0, 0));

	// handle users saving a layout to a hotkey
	if (wm_mode == SAVE) {
		save_layout(key_name);
		return;
	}

	// if key is mapped to a config command, execute that command
	if (config_command(key_name)) {
		return;
	}

	// if key is a layout defined in layouts.toml, switch to it
	if (is_workspace(key_name)) {
		switch_to_workspace(key_name);
		return;
	}
}
