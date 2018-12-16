#include "CAbGC.h"
#include "uthash.h"
// #include <string.h>

struct mapElement {
	int key; // windowID
	char value[10]; // activityID name
	UT_hash_handle hh; // make the struct hashable
};

static struct mapElement *map = NULL;

void addToMap(Window win, char* activityID) {
	struct mapElement *ele;
	int windowID = (int)win;

	// if key already exists in the map, just change 
	// the activity it points to, else add it to the map
	HASH_FIND_INT(map, &windowID, ele);
	if (ele == NULL) {
		ele = malloc(sizeof(struct mapElement));
		ele->key = windowID;
		HASH_ADD_INT(map, key, ele);
	}
	strcpy(ele->value, activityID);

}


void removeFromMap(Window win) {
	struct mapElement *ele;
	int windowID = (int)win;

	HASH_FIND_INT(map, &windowID, ele);
	HASH_DEL(map, ele);
	free(ele);
}

// return the activityID that the window is associated with
char* getActivityID(Window win) {
	struct mapElement *ele;
	int windowID = (int)win;

	HASH_FIND_INT(map, &windowID, ele);
	return ele->value;
}

// inform caller if the specified windows exists in the map
int existsInMap(Window win) {
	struct mapElement *ele;
	int windowID = (int)win;

	HASH_FIND_INT(map, &windowID, ele);
	if (ele == NULL) {
		return 0;
	}
	return 1;
}


void freeMap() {
	struct mapElement *ele, *tmp;
	HASH_ITER(hh, map, ele, tmp) {
		HASH_DEL(map, ele);  /* delete; users advances to next */
		free(ele);            /* optional- if you want to free  */
	}
}

void switchToActivity(char* activityID) {
	printf("switching to activity: %s\n", activityID);

	// don't do anything if switching to same activity
	if (!strcmp(activityID, currentActivity)) {
		return;
	}

	// unmap all mapped windows
	XUnmapSubwindows(dpy, DefaultRootWindow(dpy));

	// update currentActivity
	strcpy(currentActivity, activityID);

	struct mapElement *ele;

	// iterate through map, mapping all windows 
	// associated with the new activity
	for (ele=map; ele != NULL; ele=ele->hh.next) {
		if (!strcmp(ele->value, currentActivity)) {
			XMapWindow(dpy, ele->key);
		}
	}
}

void moveFocusedToActivity(char *activityID) {

}
