#include "CAbGC.h"
#include <X11/Xutil.h> // needed for XLookupString
// note: use xev to find keycodes for each key


// TODO go through toml to get configs
int configCommand(char *keyName) {
	// for each keyval in config.toml
	// if key->str == keyName
	// get corresponding function and execute it
	return 0;
}

/* static void keyPressInSaveMode(char *keyName) {
	switch(e->keycode) {
		case 40: savePreset("d"); return;
		case 42: savePreset("g"); return;
	}
}

void shiftKeyPress(XKeyEvent *e) {
	switch(e->keycode) {
		// 1 pressed
		case 10: moveFocusedToActivity(1); return;
		// 2 pressed
		case 11: moveFocusedToActivity(2); return;
	}
}

static void normalKeyPress(char *keyName) {

	switch(e->keycode) {
		// enter pressed
		case 36: executeCommand("urxvt"); return;
		// 1 pressed
		case 10: focusWindowByNumber(1); return;
		// 2 pressed
		case 11: focusWindowByNumber(2); return;
		// 3 pressed
		case 12: focusWindowByNumber(3); return;
		// 4 pressed
		case 13: focusWindowByNumber(4); return;
		// 5 pressed
		case 14: focusWindowByNumber(5); return;
		// 6 pressed
		case 15: focusWindowByNumber(6); return;
		// 7 pressed
		case 16: focusWindowByNumber(7); return;
		// 8 pressed
		case 17: focusWindowByNumber(8); return;
		// 9 pressed
		case 18: focusWindowByNumber(9); return;
		// 0 pressed
		case 19: focusWindowByNumber(0); return;
		// q pressed
		case 24: destroyFocusedWindow(); return;
		// s pressed
		case 39: saveMode(); return;
		// d pressed 
		case 40: switchToActivity(0); return;
		// g pressed 
		case 42: switchToActivity(1); return;
		// z pressed
		case 52: executeCommand("mpc toggle -q"); return;
		// x pressed
		case 53: executeCommand("mpc next -q"); return;
		// c pressed 
		case 54: executeCommand("nextalbum"); return;
		// b pressed 
		case 56: executeCommand("firefox"); return;
	}
} */

void keyPress(XKeyEvent *e) {
	// translate key event into the name of the key pressed as a string
	char keyName[10];
	KeySym dummy1;
	XComposeStatus dummy2;
	XLookupString(e, keyName, 10, &dummy1, &dummy2);
	printf("KEY PRESSED: %s\n", keyName);

	// handle users saving a preset to a hotkey
	if (wmMode == SAVE) {
		savePreset(keyName);
		return;
	}

	// if key is mapped to a config command, execute that command
	if (configCommand(keyName)) {
		// TODO free keyName?
		return;
	}


	// If key pressed while mod + shift held 
	if ((e->state & (ShiftMask|Mod1Mask)) == (ShiftMask|Mod1Mask)) {
		moveFocusedToActivity(keyName);
		return;
	}
	
	// if key pressed while mod held
	if ((e->state & (Mod1Mask)) == (Mod1Mask)) {
		switchToActivity(keyName);
		return;
	}

	//TODO free resources used for XLookupString?
}
