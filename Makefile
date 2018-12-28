PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall -g

all:
	$(CC) $(CFLAGS) -I$(PREFIX)/include config.c commands.c key.c main.c map.c mouse.c preset.c toml.c utils.c window.c CAbGC.h toml.h uthash.h -L$(PREFIX)/lib -lX11 -o aflatwm

install:
	test -d $(DESTDIR)/bin || mkdir -p $(DESTDIR)/bin
	install -pm 755 aflatwm $(DESTDIR)/bin

clean:
	rm -f aflatwm

