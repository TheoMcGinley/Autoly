#include "CAbGC.h"
#include <strings.h>

// quick hack - replace with writing args directly to pipe
void send_command(int argc, char **argv) {
	if (!strcmp(argv[1], "save")) {
		executeCommand("echo save > /tmp/cabgc");
	} else if (!strcmp(argv[1], "load")) {
		executeCommand("echo load > /tmp/cabgc");
	} else if (!strcmp(argv[1], "close")) {
		executeCommand("echo close > /tmp/cabgc");
	}
}

void execute_wm_command(char *command) {
	printf("command received: %s\n", command);
	executeCommand("firefox");
}
