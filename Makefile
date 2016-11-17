CC = g++

GCCFLAGS = -fpcc-struct-return
CFLAGS = -g
INCLUDE = -I/usr/include
LDFLAGS = -L/usr/lib/
LDLIBS =  -lGLU -lGL -lglut -lm

default: clean build
.PHONY: default clean build run

clean:
	rm -f manmucraft

build:
	$(CC) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) camera.cpp grid.cpp manmucraft.cpp $(LDFLAGS) $(LDLIBS) -o manmucraft

run: build
	./manmucraft
