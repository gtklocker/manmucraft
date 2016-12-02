#include <cstdlib>
#include <ctime>
#include <cmath>
#include "grid.h"
#include "debug.h"

const float DROP_TIME = 0.75f;

Cube::Cube(Color c){
	color = c;
}

Grid::Grid(int size, float tileSize) {
	m_size = size;
	m_tileSize = tileSize;
	m_colorPulse = 0;
	m_trippyMode = false;
	
	srand(time(0));
	
	grid = new Cube ***[size];
	m_trippyRand = new int **[size];
	for (int i = 0; i < size; ++i){
		grid[i] = new Cube **[size];
		m_trippyRand[i] = new int *[size];

		for (int j = 0; j < size; ++j){
			grid[i][j] = new Cube *[size];
			m_trippyRand[i][j] = new int [size];

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

				m_trippyRand[i][j][k] = rand() % 10;
			}
		}
	}

	// TODO Remove, for debugging only
	for (int j = 0; j < m_size / 2; ++j) {
		grid[j][1][2] = new Cube(RED);
	}

	// These are for testing but make sure 
	// the grid has at least size 10
	grid[3][3][3]->color = RED;
	grid[4][6][2]->color = RED;
	grid[4][7][2]->color = RED;
	grid[8][6][9]->color = RED;
	grid[7][7][7]->color = RED;
	grid[1][9][5]->color = RED;
}

Grid::~Grid() { 
	for (int i = 0; i < m_size; ++i){
		for (int j = 0;j < m_size; ++j){
			for (int k = 0; k < m_size; ++k){
				delete grid[i][j][k];
			}
			delete [] grid[i][j];
			delete [] m_trippyRand[i][j];
		}
		delete [] grid[i];
		delete [] m_trippyRand[i];
	}
	delete [] grid;
	delete [] m_trippyRand;
}	

void Grid::update(float delta) {
	if (m_layersToDrop > 0) {
		m_dropTimer += delta;
		if (m_dropTimer >= DROP_TIME) {
			dropLayer();
			m_layersToDrop--;
			m_dropTimer -= DROP_TIME;
		}
	}

	m_colorPulse += delta;
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

	float laxTileSize = 0.707106781 * m_tileSize;

	RealCoords rb = transformGreedyToReal(b);

	if (abs(a.y - rb.y) > m_tileSize / 2) {
		return false;
	}
	if (abs(a.x - rb.x) * abs(a.x - rb.x) + abs(a.z - rb.z) * abs(a.z - rb.z) < laxTileSize * laxTileSize) {
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

				float o = 0.95;
				if (m_trippyMode) {
					float tripFactor = (1.0 + sin(m_colorPulse * ((1.0 + sin(m_trippyRand[i][j][k])) / 2.0))) / 2.0;
					o = .2 + tripFactor;
					c -= -.2 + tripFactor;
				}
				switch (grid[i][j][k]->color) {
					case EMPTY:
						continue;
					case YELLOW:
						glColor4f(.90 + c, .67 + c, .04 + c, o);
						break;
					case RED:
						glColor4f(.97 + c, .03 + c, .03 + c, o);
						break;
					case BLUE:
						glColor4f(0.0 + c, 0.0 + c, 1.0 + c, o);
						break;
					case GREEN:
						glColor4f(0.0 + c, 1.0 + c, 0.0 + c, o);
						break;
					case MAGENTA:
						glColor4f(.71 + c, .11 + c, .76 + c, o);
						break;
					default:
						break;
				}

				glPushMatrix();
				glTranslatef(0.0, 0.0, k * m_tileSize);
				glutSolidCube(m_tileSize);
				DEBUG({
					glColor3f(0.0, 1.0, 1.0);
					glutWireCube(m_tileSize);
				});
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

void Grid::dropCubes() {
	m_layersToDrop = m_size - 1;
	m_dropTimer = 0.0f;
}

void Grid::dropLayer() {
	for (int i = 0; i < m_layersToDrop; i++) {
		for (int j = 0; j < m_size; j++) {
			for (int k = 0; k < m_size; k++) {
				Cube *cur = grid[j][i][k];
				Cube *top = grid[j][i + 1][k];
				if (cur->color == EMPTY && top->color != EMPTY){
					cur->color = top->color;
					top->color = EMPTY;
				}
			}
		}
	}
}

float Grid::getCubeSize() {
	return m_tileSize;
}

bool Grid::isGreedyCoordValid(int coord) {
	return coord >= 0 && coord < m_size;
}

bool Grid::placeCube(RealCoords pos, Color c) {
	GreedyCoords posg = transformRealToGreedy(pos);
	for (int i = 0; i < m_size; i++) {
		if (grid[posg.x][i][posg.z]->color == EMPTY) {
			grid[posg.x][i][posg.z]->color = c;
			return true;
		}
	}
	return false;
}

void Grid::kickCube(GreedyCoords start, GreedyCoords direction) {
	direction.x = direction.x != 0 ? (direction.x / abs(direction.x)) : 0;
	direction.z = direction.z != 0 ? (direction.z / abs(direction.z)) : 0;

	Cube *prevCube = new Cube(EMPTY);
	for (int x = start.x, z = start.z; isGreedyCoordValid(x) && isGreedyCoordValid(z); x -= direction.x, z -= direction.z) {
		Cube *cb = getCubeAtGreedy((GreedyCoords){x, start.y, z});
		Cube **cbPtr = &cb;

		grid[x][start.y][z] = prevCube;
		prevCube = *cbPtr;
	}
	delete prevCube;
}

int Grid::getGridSize() {
	return m_size;
}

void Grid::toggleTrippy() {
	m_trippyMode = !m_trippyMode;
}
