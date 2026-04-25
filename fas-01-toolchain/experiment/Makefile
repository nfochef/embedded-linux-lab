CROSS_COMPILE ?=
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -O2

all: hello-dyn hello-static

hello-dyn: hello.c
	$(CC) $(CFLAGS) -o $@ $< 

hello-static: hello.c
	$(CC) $(CFLAGS) -static -o $@ $< 

clean:
	rm -f hello hello-dyn hello-static

.PHONY: all clean

