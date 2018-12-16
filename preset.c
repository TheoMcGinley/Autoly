#include "CAbGC.h"
#include "toml.h"
#include <stdio.h> // for file manipulation
#include <X11/Xutil.h> //for XClassHint

void saveMode() {
	wmMode = SAVE;
}

// if a preset has already been defined for
// the given hotkey, remove it from presets.toml
void removeExistingPreset(const char *hotkey) {

	FILE *fp;
	fp = fopen("/home/theo/dev/CAbGC/presets.toml", "a");

	char buffer[300];
	int startLine = -1;

	// for each line of presets.toml
	for (int line = 0; fgets(buffer, sizeof(buffer), fp); line++) {
		printf("buffer: %s\n", buffer);

		// check if current line is equal to [<hotkey>], indicating start of
		// an existing preset for the given hotkey
		char hotkeyString[20];
		snprintf(hotkeyString, sizeof(hotkeyString), "[%s]", hotkey);
		printf("hotkeyString: %s\n", hotkeyString);
		if (!strcmp(buffer, hotkeyString)) {
			startLine = line;
		}
		// TODO free hotkeyString?
	}

	// if no existing preset found, return
	if (startLine == -1) {
		return;
	}

	rewind(fp);
	FILE *tmpFile = fopen("/home/theo/dev/CAbGC/tmp.toml", "a");

	// write other presets until the preset-to-remove to a tmp file
	for (int i = 0; i < startLine; i++) {
		fgets(buffer, sizeof(buffer), fp); 
		fprintf(tmpFile, "%s\n", buffer);
	}

	// until a new preset or EOF reached, move through lines without writing
	// them to the tmp file (crude form of line deletion)
	// this works as all lines except new preset declarations start with a tab
	while (strncmp(buffer, "[", 1) || fgets(buffer, sizeof(buffer), fp)) {
		fgets(buffer, sizeof(buffer), fp); 
	}


	// copy the rest of the presets to the tmp file
	while (fgets(buffer, sizeof(buffer), fp)) {
		fprintf(tmpFile, "%s\n", buffer);
	}

	// replace old presets file with new presets file
	fclose(tmpFile);
	fclose(fp);
	remove("/home/theo/dev/CAbGC/presets.toml");
	rename("/home/theo/dev/CAbGC/tmp.toml", "/home/theo/dev/CAbGC/presets.toml");
}


struct Application *loadApplication(TomlTable *table) {

	TomlErr err = TOML_ERR_INIT;
	TomlTableIter *it = toml_table_iter_new(table, &err);
	if (err.code != TOML_OK) {
		toml_clear_err(&err);
		toml_table_iter_free(it);
		return NULL;
	}


	struct Application *app = malloc (sizeof (app));

	// each value of the keyvals refers to an attribute of the window
	// assign the attribute to the relevant variable
	while (toml_table_iter_has_next(it)) {
		TomlKeyValue *keyval = toml_table_iter_get(it);
		printf("checking key of: %s\n", keyval->key->str);

		// really wish C could switch on strings...
		if (!strcmp(keyval->key->str, "wm_class")) {
			printf("assigning value of %s to wm_class: \n", keyval->value->value.string->str);
			app->wm_class = keyval->value->value.string->str;
		} else if (!strcmp(keyval->key->str, "width")) {
			printf("assigning value of %ld to width: \n", keyval->value->value.integer);
			app->width = keyval->value->value.integer;
		} else if (!strcmp(keyval->key->str, "height")) {
			printf("assigning value of %ld to height: \n", keyval->value->value.integer);
			app->height = keyval->value->value.integer;
		} else if (!strcmp(keyval->key->str, "x")) {
			printf("assigning value of %ld to x: \n", keyval->value->value.integer);
			app->x = keyval->value->value.integer;
		} else if (!strcmp(keyval->key->str, "y")) {
			printf("assigning value of %ld to y: \n", keyval->value->value.integer);
			app->y = keyval->value->value.integer;
		}

		toml_table_iter_next(it);
	}


	printf("load app iter free\n");
	toml_table_iter_free(it);
	return app;

}

// t is the table referring to the activity (e.g. [d])
struct Preset *loadPreset(TomlTable *table) {

	TomlErr err = TOML_ERR_INIT;
	TomlTableIter *it = toml_table_iter_new(table, &err);
	if (err.code != TOML_OK) {
		printf("loadpreset (singular) error and iter free\n");
		toml_clear_err(&err);
		toml_table_iter_free(it);
		return NULL;
	}

	// create the head of the application list and save the 
	// start of the list for later
	struct Application *newApp = malloc (sizeof(newApp));
	struct Application *applicationList = newApp;

	// each value of the keyvals is a table referring to one of the 
	// applications in the preset (e.g. [d.window1])
	while (toml_table_iter_has_next(it)) {
		printf("LOADPRESET (SINGULAR) ITER\n");
		TomlKeyValue *keyval = toml_table_iter_get(it);

		// append application to end of list
		newApp->next = loadApplication(keyval->value->value.table);
		newApp = newApp->next;

		toml_table_iter_next(it);
	}

	// return the preset
	struct Preset *preset = malloc (sizeof (struct Preset));
	// preset->hotkey = hotkey;
	preset->applicationList = *applicationList;
	preset->next = NULL;

	printf("loadpreset (singular) iter free\n");
	toml_table_iter_free(it);

	return preset;
}

void loadPresets() {

	// load presets.toml into table
	TomlErr err = TOML_ERR_INIT;
	TomlTable *table = toml_load_filename("/home/theo/dev/CAbGC/presets.toml", &err);
	if (err.code != TOML_OK) goto cleanuptable;

	TomlTableIter *it = toml_table_iter_new(table, &err);
	if (err.code != TOML_OK) goto cleanupiter;

	// used for appending to list of presets
	struct Preset *currentPreset = &presets;

	// for each preset (e.g. [d]) in presets.toml...
	while (toml_table_iter_has_next(it)) {
		TomlKeyValue *keyval = toml_table_iter_get(it);
		// build the preset structure
		// struct Preset *preset = loadPreset(keyval->value->table);
		struct Preset *preset = loadPreset(keyval->value->value.table);
		preset->hotkey =  keyval->key->str;

		// append preset to end of list
		currentPreset->next = preset;
		currentPreset = currentPreset->next;

		toml_table_iter_next(it);
	}

	// free all used resources
	cleanupiter:
		printf("loadpresets iter free\n");
		toml_table_iter_free(it);

	cleanuptable:
		printf("loadpresets table free\n");
		// TODO toml_table_free(table);
		printf("finished freeing table\n");
		if (err.code != TOML_OK) {
			printf("ERROR NOT OK\n");
			fprintf(stderr, "toml: %d: %s\n", err.code, err.message);
			toml_clear_err(&err);
		}

	printf("finished loading presets, no errors\n");
}

// TODO use XGetClassHint
char *getWMclass(Window win) {

	// hint.res_name = application name
	// hint.res_class = application name
	XClassHint hint;
	int status = XGetClassHint(dpy, win, &hint);

	if (status != Success) {
		XFree(hint.res_name);
		XFree(hint.res_class);
		return NULL;
	}

	XFree(hint.res_name);
	return hint.res_class;
}

// save current layout to specified hotkey
void savePreset(const char *hotkey) {

	printf("saving preset!\n");

	removeExistingPreset(hotkey);

	FILE *fp;
	fp = fopen("/home/theo/dev/CAbGC/presets.toml", "a");

	// get file ready to enter each window
	fprintf(fp, "[%s]\n", hotkey);

	// get all mapped windows
	Window dummy1, dummy2, *windowList;
	unsigned int numWindows;
	XQueryTree(dpy, DefaultRootWindow(dpy), &dummy1, &dummy2, &windowList, &numWindows);
	
	// write the properties of each mapped windows to presets.toml
	for (int i=0; i<numWindows; i++) {
		int x, y;
		unsigned int width, height, dummy3;
		int status = XGetGeometry(dpy, windowList[i], &dummy1, &x, &y, &width, &height, &dummy3, &dummy3);
		if (status != Success) goto cleanup;
		char *wm_class = getWMclass(windowList[i]);

		fprintf(fp, "\t[%s.window%d]\n", hotkey, i+1);
		fprintf(fp, "\t# load_script = \"...\"\n", hotkey, i+1);
		fprintf(fp, "\twm_class = \"%s\"\n", wm_class);
		fprintf(fp, "\twidth = \"%d\"\n", width);
		fprintf(fp, "\theight = \"%d\"\n", height);
		fprintf(fp, "\tx = \"%d\"\n", x);
		fprintf(fp, "\ty = \"%d\"\n\n", y);

		XFree(wm_class);
	}


cleanup:
	XFree(windowList);
	fclose(fp);
	wmMode = NORMAL;
}
