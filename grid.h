#pragma once
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

class Grid {

private:
	int m_size;
	float m_tileSize;
public:
	Grid(int size, float tileSize);
	~Grid();

	void update(float delta);
	void render();

};
