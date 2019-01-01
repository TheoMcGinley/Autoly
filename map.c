#include "autoly.h"
#include "uthash.h"

typedef struct _MapElement {
	int key; // windowID
	char value[10]; // workspace name
	UT_hash_handle hh; // make the struct hashable
} MapElement;

static MapElement *map = NULL;

void add_to_map(Window win, char* workspace) {
	MapElement *ele;
	int window_id = (int)win;

	// if key already exists in the map, just change 
	// the workspace it points to, else add it to the map
	HASH_FIND_INT(map, &window_id, ele);
	if (ele == NULL) {
		ele = malloc(sizeof(MapElement));
		ele->key = window_id;
		HASH_ADD_INT(map, key, ele);
	}
	strcpy(ele->value, workspace);
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

// return the workspace ID that the window is associated with
char* get_workspace_id(Window win) {
	MapElement *ele;
	int window_id = (int)win;

	HASH_FIND_INT(map, &window_id, ele);
	return ele->value;
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

void switch_to_workspace(char* workspace_id) {
	printf("switching to workspace: %s\n", workspace_id);

	// don't do anything if switching to same workspace
	if (!strcmp(workspace_id, current_workspace)) {
		return;
	}

	// unmap all mapped windows
	XUnmapSubwindows(dpy, DefaultRootWindow(dpy));

	// update currentworkspace
	strcpy(current_workspace, workspace_id);

	MapElement *ele;

	// iterate through map, mapping all windows 
	// associated with the new workspace
	for (ele=map; ele != NULL; ele=ele->hh.next) {
		if (!strcmp(ele->value, current_workspace)) {
			XMapWindow(dpy, ele->key);
		}
	}
}

void move_focused_to_workspace(char *workspace_id) {

}
