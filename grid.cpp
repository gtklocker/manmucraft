#include <cstdlib>
#include <ctime>
#include "grid.h"

Cube::Cube(Type t){
	type = t;
}

Grid::Grid(int size, float tileSize) {
	m_size = size;
	m_tileSize = tileSize;
	
	srand(time(0));
	
	grid = new Cube ***[size];
	for (int i = 0; i < size; ++i){
		grid[i] = new Cube **[size];
		for (int j = 0; j < size; ++j){
			grid[i][j] = new Cube *[size];
			for (int k = 0; k < size; ++k){
				if (j == 0){
					grid[i][j][k] = new Cube(DIRT);
				}
				else {
					grid[i][j][k] = new Cube(EMPTY);
				}
			}
		}
	}
}

Grid::~Grid() { 
	for (int i = 0; i < m_size; ++i){
		for (int j = 0;j < m_size; ++j){
			for (int k = 0; k < m_size; ++k){
				delete grid[i][j][k];
			}
			delete [] grid[i][j];
		}
		delete [] grid[i];
	}
	delete [] grid;
}	

void Grid::update(float delta) {

}

void Grid::render() {

	for (int i = 0; i < m_size; ++i) {
		glPushMatrix();
		glTranslatef(i * m_tileSize, 0.0, 0.0);
		for (int j = 0; j < m_size; ++j) {
			glPushMatrix();
			glTranslatef(0.0, j * m_tileSize, 0.0);
			for (int k = 0; k < m_size; ++k) {
				switch (grid[i][j][k]->type) {
					case EMPTY:
						continue;
					case DIRT:
						glColor3f(0.5, 0.25, 0.0);
						break;
					case GRASS:
						glColor3f(0.0, 0.75, 0.0);
						break;
					default:
						break;
				}

				glPushMatrix();
				glTranslatef(0.0, 0.0, k * m_tileSize);
				glutSolidCube(m_tileSize);
				glColor3f(0.0, 1.0, 1.0);
				glutWireCube(m_tileSize);
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
}
