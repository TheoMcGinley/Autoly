PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall -g

all:
	$(CC) $(CFLAGS) -I$(PREFIX)/include config.c commands.c key.c main.c map.c mouse.c layout.c toml.c utils.c window.c autoly.h toml.h uthash.h -L$(PREFIX)/lib -lX11 -o autoly

install:
	test -d $(DESTDIR)/bin || mkdir -p $(DESTDIR)/bin
	install -pm 755 autoly $(DESTDIR)/bin

clean:
	rm -f autoly

