.POSIX:
.SUFFIXES:

CC = cc
CFLAGS = -Wall -Werror
LDFLAGS =
LDLIBS = -lpam -lcursedpotato
PREFIX = /usr/local

all: build/dnlm

build/dnlm: build/display.o build/start.o build/bigfont.o build/auth.o build/main.o
	$(CC) $(CFLAGS) -o $@ build/display.o build/start.o build/bigfont.o build/auth.o build/main.o $(LDFLAGS) $(LDLIBS)

build/display.o: src/display.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

build/start.o: src/start.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

build/bigfont.o: src/bigfont.c
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
	cp -f build/dnlm $(PREFIX)/bin/
	cp -f res/dnlm.openrc /etc/init.d/dnlm
	cp -f res/dnlm.pam.conf /etc/pam.d/dnlm
