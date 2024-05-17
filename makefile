.POSIX:
.SUFFIXES:

CC = cc
CFLAGS = -Wall -Werror
LDFLAGS = 
LDLIBS = -lpam -lcinderpelt
PREFIX = /usr/local

all: build/whitestorm

build/whitestorm: build/display.o build/start.o build/auth.o build/main.o
	$(CC) $(CFLAGS) -o $@ build/display.o build/start.o build/auth.o build/main.o $(LDFLAGS) $(LDLIBS)

build/display.o: src/display.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

build/start.o: src/start.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

build/auth.o: src/auth.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

build/main.o: src/main.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf build

install: all
	cp build/whitestorm $(PREFIX)/bin/
