CC = g++

GCCFLAGS = -fpcc-struct-return
CFLAGS = -g
INCLUDE = -I/usr/include
LDFLAGS = -L/usr/lib/
LDLIBS =  -lGLU -lGL -lglut -lm

default: clean build
.PHONY: default clean build run test

clean:
	rm -f manmucraft grid_test

build:
	$(CC) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) lighting.cpp player.cpp grid.cpp manmucraft.cpp $(LDFLAGS) $(LDLIBS) -o manmucraft

grid_test:
	$(CC) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) grid.cpp grid_test.cpp $(LDFLAGS) $(LDLIBS) -o grid_test

test: grid_test
	./grid_test

run: build
	./manmucraft
