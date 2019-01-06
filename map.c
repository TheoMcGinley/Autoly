#include "autoly.h"
#include "uthash.h"

typedef struct _MapElement {
	int key; // windowID
	char value[10]; // hotkey name
	UT_hash_handle hh; // make the struct hashable
} MapElement;

static MapElement *map = NULL;

// associate window with hotkey
void add_to_map(Window win, char* hotkey) {
	MapElement *ele;
	int window_id = (int)win;

	// if key already exists in the map, just change 
	// the hotkey it points to, else add it to the map
	HASH_FIND_INT(map, &window_id, ele);
	if (ele == NULL) {
		ele = malloc(sizeof(MapElement));
		ele->key = window_id;
		HASH_ADD_INT(map, key, ele);
	}
	strcpy(ele->value, hotkey);
}


void remove_from_map(Window win) {
	MapElement *ele;
	int window_id = (int)win;

	HASH_FIND_INT(map, &window_id, ele);
	// don't try to delete non-existant window
	if (ele == NULL) {
		return;
	}
	HASH_DEL(map, ele);
	free(ele);
}

// return the hotkey associated with the given window
char* get_hotkey(Window win) {
	MapElement *ele;
	int window_id = (int)win;

	HASH_FIND_INT(map, &window_id, ele);
	return ele->value;
}

// return a pointer to layout of hotkey, or
// NULL if no layout with the given hotkey exists
Layout* get_layout(char *hotkey) {
	Layout *l = &layouts;
	while (l->next != NULL) {
		l = l->next;
		if (!strcmp(l->hotkey, hotkey)) {
			return l;
		}
	}
	return NULL;
}

// inform caller if the specified windows exists in the map
int exists_in_map(Window win) {
	MapElement *ele;
	int window_id = (int)win;

	HASH_FIND_INT(map, &window_id, ele);
	if (ele == NULL) {
		return 0;
	}
	return 1;
}


void free_map() {
	MapElement *ele, *tmp;
	HASH_ITER(hh, map, ele, tmp) {
		HASH_DEL(map, ele);  /* delete; users advances to next */
		free(ele);            /* optional- if you want to free  */
	}
}

void switch_to_layout(char* hotkey) {
	printf("switching to layout: %s\n", hotkey);

	// don't do anything if switching to current layout
	if (!strcmp(hotkey, current_layout->hotkey)) {
		return;
	}

	// unmap all mapped windows
	XUnmapSubwindows(dpy, DefaultRootWindow(dpy));

	// update current layout
	current_layout = get_layout(hotkey);

	MapElement *ele;

	// iterate through map, mapping all windows 
	// associated with the new layout
	for (ele=map; ele != NULL; ele=ele->hh.next) {
		if (!strcmp(ele->value, current_layout->hotkey)) {
			XMapWindow(dpy, ele->key);
		}
	}
}

void move_focused_to_layout(char *hotkey) {

}
