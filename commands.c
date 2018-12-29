#include "CAbGC.h"
#include <strings.h>
#include <pthread.h>

#define CASE(c) if (!strcmp(command, c))


// handle all incoming commands
void *listen (void *ptr) {
	char *pipe = PIPE_FILE;
	mkfifo(pipe, 0666);
	// int pipe_fd = open(pipe, O_RDONLY | O_NONBLOCK);
	int pipe_fd = open(pipe, O_RDONLY);

	// process message to wm (thanks hootwm!)
	char buffer[511];
	int length;

	// TODO sleep so it doesn't thrash CPU?
	for(;;) {
		if (length = read(pipe_fd, buffer, sizeof(buffer))) {
			printf("listener has heard, length: %d\n", length);
			buffer[length-1] = '\0';
			printf("listener received: %s\n", buffer);
			execute_wm_command(buffer);
		}
	}

	return NULL;
}

// listen for commands asynchronously
void listen_for_commands() {
	pthread_t thread;
	pthread_create(&thread, NULL, listen, NULL);
}



// quick hack - replace with writing args directly to pipe
void send_command(int argc, char **argv) {
	char *command = argv[1];
	CASE("save") {
		executeCommand("echo save > /tmp/cabgc");
	} else CASE("load") {
		executeCommand("echo load > /tmp/cabgc");
	} else CASE("close") {
		executeCommand("echo close > /tmp/cabgc");
	}
}

void execute_wm_command(char *command) {
	printf("command received: %s\n", command);

	CASE("save") {
		printf("entering save mode...\n");
		saveMode();
	} else CASE("load") {
		printf("loading applications...\n");
	} else CASE("close") {
		printf("closing focused window...\n");
		destroyFocusedWindow();
		printf("finished closing focused window\n");
	}
}
