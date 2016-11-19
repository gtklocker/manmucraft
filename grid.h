#pragma once
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

enum Type {
	GRASS, DIRT, EMPTY
};

struct Cube {
	Type type;	
	Cube(Type t);
};

struct GreedyCoords {
	int x;
	int y;
	int z;
};

struct RealCoords {
	float x;
	float y;
	float z;
};

class Grid {
private:
	int m_size;
	float m_tileSize;
	Cube ****grid;
public:
	Grid(int size, float tileSize);
	~Grid();

	void update(float delta);
	void render();

	RealCoords transformGreedyToReal(GreedyCoords greedy);
	GreedyCoords transformRealToGreedy(RealCoords real);
	Cube* getCubeAtReal(RealCoords real);
	Cube* getCubeAtGreedy(GreedyCoords greedy);
};
