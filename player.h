#pragma once
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "grid.h"

class Player {
private:
	float m_x, m_y, m_z, m_angle, m_pitch;
	bool m_firstPerson;
	Grid *m_grid;
public:
	Player(float x, float y, float z, Grid *grid);
	~Player();
	void update(float delta);
	void render();
	void turnLeft();
	void turnRight();
	void lookUp();
	void lookDown();
	void moveForward();
	void moveBackwards();
	void toggleCameraView();
	void updateView();
};
