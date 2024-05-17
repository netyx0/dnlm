.POSIX:
.SUFFIXES:

CC = cc
CFLAGS = -Wall -Werror
LDFLAGS =
LDLIBS = -lpam
PREFIX = /usr/local

all: build/whitestorm

build/whitestorm: build/display.o build/auth.o build/main.o
	$(CC) $(CFLAGS) -o $@ build/display.o build/auth.o build/main.o $(LDFLAGS) $(LDLIBS)

build/display.o: src/display.c
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
	install -o root -g root -m 755 build/whitestorm $(PREFIX)/bin/
	install -o root -g root -m 611 res/whitestorm.pam.conf /etc/pam.d/whitestorm
	install -o root -g root -m 766 res/whitestorm.openrc /etc/init.d
