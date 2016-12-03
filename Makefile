CXX ?= g++
GCCFLAGS = -fpcc-struct-return -std=c++11
CFLAGS = -g
INCLUDE = -I/usr/include
LDFLAGS = -L/usr/lib/
LDLIBS =  -lGLU -lGL -lglut -lm

default: clean build
.PHONY: default clean build run test

clean:
	rm -f manmucraft grid_test

build:
	$(CXX) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) lighting.cpp player.cpp grid.cpp text.cpp manmucraft.cpp $(LDFLAGS) $(LDLIBS) -o manmucraft

grid_test:
	$(CXX) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) grid.cpp grid_test.cpp $(LDFLAGS) $(LDLIBS) -o grid_test

test: grid_test
	./grid_test

run: build
	./manmucraft ${N}
