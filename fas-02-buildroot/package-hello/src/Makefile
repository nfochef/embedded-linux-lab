CC ?= gcc
CFLAGS ?= -Wall -O2

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

clean:
	rm -f hello

install:
	install -D -m 0755 hello $(DESTDIR)/usr/bin/hello

.PHONY: clean install
