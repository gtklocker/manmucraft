#include <cstdlib>
#include <ctime>
#include "grid.h"

Grid::Grid(int size, float tileSize) {
	m_size = size;
	m_tileSize = tileSize;
	
	srand(time(0));
}

Grid::~Grid() { }

void Grid::update(float delta) {

}

void Grid::render() {

	for (int i = 0; i < m_size; ++i) {
		glPushMatrix();
		glTranslatef(i * m_tileSize, 0.0, 0.0);
		for (int j = 0; j < 1; ++j) {
			glPushMatrix();
			glTranslatef(0.0, j * m_tileSize, 0.0);
			for (int k = 0; k < m_size; ++k) {
				glPushMatrix();
				glTranslatef(0.0, 0.0, k * m_tileSize);
				glColor3f(0.0, 1.0, 1.0);
				glutSolidCube(m_tileSize);
				glColor3f(1.0, 0.0, 0.0);
				glutWireCube(m_tileSize);
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
}
