PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall

all:
	$(CC) $(CFLAGS) -I$(PREFIX)/include key.c main.c map.c mouse.c utils.c window.c CAbGC.h uthash.h -L$(PREFIX)/lib -lX11 -o aflatwm

install:
	test -d $(DESTDIR)/bin || mkdir -p $(DESTDIR)/bin
	install -pm 755 aflatwm $(DESTDIR)/bin

clean:
	rm -f aflatwm

