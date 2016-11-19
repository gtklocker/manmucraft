#pragma once
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

class Player {
private:
	float m_x, m_y, m_z, m_angle, m_pitch;
	bool m_firstPerson;
public:
	Player(float x, float y, float z);
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
