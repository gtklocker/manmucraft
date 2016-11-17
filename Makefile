CC = g++

GCCFLAGS = -fpcc-struct-return
CFLAGS = -g
INCLUDE = -I/usr/include
LDFLAGS = -L/usr/lib/
LDLIBS =  -lGLU -lGL -lglut -lm


manmucraft:
	$(CC) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) camera.cpp grid.cpp manmucraft.cpp $(LDFLAGS) $(LDLIBS) -o manmucraft

default:
	manmucraft
