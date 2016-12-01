#include <cstdlib>
#include <ctime>
#include <cmath>
#include "grid.h"

Cube::Cube(Color c){
	color = c;
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
				Color randomColor = Color(rand() % MAGENTA);
				if (j == 0) {
					grid[i][j][k] = new Cube(randomColor);
					if (i == size / 2 && k == size / 2) {
						grid[i][j][k] = new Cube(MAGENTA);
					}
				}
				else {
					grid[i][j][k] = new Cube(EMPTY);
				}
			}
		}
	}

	// TODO Remove, for debugging only
	for (int j = 0; j < m_size / 2; ++j) {
		grid[j][1][2] = new Cube(RED);
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

// We having two systems of coordinates here,
//  - Real which is the system OpenGL uses
//  - Greedy which is the system our grid works with
RealCoords Grid::transformGreedyToReal(GreedyCoords greedy) {
	return (RealCoords){
		greedy.x * m_tileSize,
		greedy.y * m_tileSize,
		greedy.z * m_tileSize
	};
}

GreedyCoords Grid::transformRealToGreedy(RealCoords real) {
	return (GreedyCoords){
		(int) ((real.x + m_tileSize / 2) / m_tileSize),
		(int) ((real.y + m_tileSize / 2) / m_tileSize),
		(int) ((real.z + m_tileSize / 2) / m_tileSize)
	};
}

Cube* Grid::getCubeAtReal(RealCoords real) {
	return getCubeAtGreedy(transformRealToGreedy(real));
}

Cube* Grid::getCubeAtGreedy(GreedyCoords greedy) {
	if (greedy.x < 0 || greedy.x >= m_size ||
		greedy.y < 0 || greedy.y >= m_size ||
		greedy.z < 0 || greedy.z >= m_size) {
		return NULL;
	}
	return grid[greedy.x][greedy.y][greedy.z];
}

bool Grid::doCollide(RealCoords a, GreedyCoords b) {
	Cube *cubeThere = getCubeAtGreedy(b);
	if (cubeThere == NULL || (cubeThere != NULL && cubeThere->color == EMPTY)) {
		return false;
	}

	float laxTileSize = 1.25 * m_tileSize;

	RealCoords rb = transformGreedyToReal(b);
	if (abs(a.x - rb.x) * abs(a.x - rb.x) + abs(a.z - rb.z) * abs(a.z - rb.z) <= laxTileSize * laxTileSize) {
		return true;
	}

	return false;
}

void Grid::render() {
	for (int i = 0; i < m_size; ++i) {
		glPushMatrix();
		glTranslatef(i * m_tileSize, 0.0, 0.0);
		for (int j = 0; j < m_size; ++j) {
			glPushMatrix();
			glTranslatef(0.0, j * m_tileSize, 0.0);
			for (int k = 0; k < m_size; ++k) {
				if (grid[i][j][k] == NULL) {
					continue;
				}

				float c = grid[i][j][k]->isChosen ? .35 : 0;
				switch (grid[i][j][k]->color) {
					case EMPTY:
						continue;
					case YELLOW:
						glColor3f(.90 + c, .67 + c, .04 + c);
						break;
					case RED:
						glColor3f(.97 + c, .03 + c, .03 + c);
						break;
					case BLUE:
						glColor3f(0.0 + c, 0.0 + c, 1.0 + c);
						break;
					case GREEN:
						glColor3f(0.0 + c, 1.0 + c, 0.0 + c);
						break;
					case MAGENTA:
						glColor3f(.71 + c, .11 + c, .76 + c);
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

void Grid::removeColumn(RealCoords col) {
	GreedyCoords colg = transformRealToGreedy(col);
	for (int i = 0; i < m_size; i++) {
		if (grid[colg.x][i][colg.z]->color != MAGENTA) {
			grid[colg.x][i][colg.z]->color = EMPTY;
		}
	}
}

float Grid::getCubeSize() {
	return m_tileSize;
}

bool Grid::isGreedyCoordValid(int coord) {
	return coord >= 0 && coord < m_size;
}

void Grid::kickCube(GreedyCoords start, GreedyCoords direction) {
	direction.x = direction.x != 0 ? (direction.x / abs(direction.x)) : 0;
	direction.z = direction.z != 0 ? (direction.z / abs(direction.z)) : 0;

	Cube *prevCube = NULL;
	for (int x = start.x, z = start.z; isGreedyCoordValid(x) && isGreedyCoordValid(z); x -= direction.x, z -= direction.z) {
		Cube *cb = getCubeAtGreedy((GreedyCoords){x, start.y, z});
		Cube **cbPtr = &cb;

		grid[x][start.y][z] = prevCube;
		prevCube = *cbPtr;
	}
}

int Grid::getGridSize() {
	return m_size;
}
