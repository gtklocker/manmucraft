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

};
