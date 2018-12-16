#include "CAbGC.h"
#include "toml.h"
// #include <stdio.h> // for file manipulation
#include <X11/Xutil.h> //for XClassHint

typedef void (*configFunction)();

// ugly mapping of function names to the relevant function
configFunction getConfigFunction(char *funcName) {
	printf("FUNC NAME: %s\n", funcName);
	if (!strcmp(funcName, "save layout")) {
		return &savePreset;
	} else if (!strcmp(funcName, "load layout")) {
		return &loadLayout;
	} else if (!strcmp(funcName, "close focused window")) {
		return &destroyFocusedWindow;
	}
	// TODO remove this default return
	return &destroyFocusedWindow;
}

void loadConfig() {

	// load config.toml into table
	TomlErr err = TOML_ERR_INIT;
	TomlTable *table = toml_load_filename("/home/theo/dev/CAbGC/config.toml", &err);
	if (err.code != TOML_OK) goto cleanuptable;

	TomlTableIter *it = toml_table_iter_new(table, &err);
	if (err.code != TOML_OK) goto cleanupiter;

	// used for appending to list of keybinds
	struct Keybind *currentKeybind = &keybinds;

	// for each config (e.g. s = saveLayout) in config.toml...
	while (toml_table_iter_has_next(it)) {
		TomlKeyValue *keyval = toml_table_iter_get(it);

		struct Keybind *kb = malloc (sizeof(kb));
		kb->hotkey = keyval->key->str;
		kb->functionPointer = getConfigFunction(keyval->value->value.str->str);

		// append keybind to end of list
		currentKeybind->next = preset;
		currentKeybind = currentKeybind->next;

		toml_table_iter_next(it);
	}

	// free all used resources
	cleanupiter:
		printf("loadConfig iter free\n");
		toml_table_iter_free(it);

	cleanuptable:
		printf("loadConfig table free\n");
		// TODO toml_table_free(table);
		printf("finished freeing table\n");
		if (err.code != TOML_OK) {
			printf("ERROR NOT OK\n");
			fprintf(stderr, "toml: %d: %s\n", err.code, err.message);
			toml_clear_err(&err);
		}

	printf("finished loading keybinds, no errors\n");
}
