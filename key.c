#include "CAbGC.h"
// note: use xev to find keycodes for each key


// TODO keycode to string conversion
static void keyPressInSaveMode(XKeyEvent *e) {
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

static void normalKeyPress(XKeyEvent *e) {
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
}

void keyPress(XKeyEvent *e) {
	if (wmMode == SAVE) {
		keyPressInSaveMode(e);
		return;
	}

	// If key pressed while mod + shift held
	if ((e->state & (ShiftMask|Mod1Mask)) == (ShiftMask|Mod1Mask)) {
		shiftKeyPress(e);
		return;
	}
	
	// if key pressed while mod held
	if ((e->state & (Mod1Mask)) == (Mod1Mask)) {
		normalKeyPress(e);
	}
}

