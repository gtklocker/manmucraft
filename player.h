#pragma once
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <vector>

#include "grid.h"

using namespace std;

class Player {
private:
	float m_x, m_y, m_z, m_angle, m_pitch;
	float m_startX, m_startY, m_startZ;
	float m_ySpeed;
	int m_currentLevel, m_levelReached, m_lastLevel;
	int m_lives, m_points;
	bool m_movingForward, m_movingBackwards;
	bool m_firstPerson;
	bool m_jumping;
	Grid *m_grid;
	Cube *m_chosen;
	RealCoords m_chosenCoords;
	vector<Cube> m_reserve;

	void moveWithDirection(int direction, float delta);
	void moveTo(RealCoords coords);
	bool canMoveTo(RealCoords coords);
	void death();
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
	void stopMovingForward();
	void moveBackwards();
	void stopMovingBackwards();
	void jump();
	void pickUpCube();
	void placeCube();
	void kickCube();
	void removeColumn();
	void respawn();
	void toggleCameraView();
	void updateView();

	char* getHUDString();
};
