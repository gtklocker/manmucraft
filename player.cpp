#include <iostream>
#include <cmath>
#include <cassert>

#include "player.h"
#include "grid.h"
#include "debug.h"
#define _USE_MATH_DEFINES

float toDegrees(float rads) {
	return rads * 180 / M_PI;
}

float distanceRealCoords(RealCoords &r1, RealCoords &r2) {
	return sqrt(pow(r1.x - r2.x, 2) + pow(r1.y - r2.y, 2) + pow(r1.z - r2.z, 2));
}

const float MOVE_ANGLE = .025;
const float MOVE_SPEED = 1.0;
const float TP_RADIUS = 1.5;
const float FP_RADIUS = 0.50;
const float RAY_LENGTH = 2.0;
const float PITCH_UP_LIMIT = 1.5;
const float PITCH_DOWN_LIMIT = -3.0;
const float Y_DIST = .125;
const float JUMP_SPEED = 3.0;
const float PLAYER_HEIGHT = 0.9;
const float G = 9.807 / 5;
const float PLACE_DISTANCE = 1.75f;

Player::Player(float x, float y, float z, Grid *grid) {
	m_x = x;
	m_y = y;
	m_z = z;
	m_grid = grid;

	m_angle = 0.0;
	m_firstPerson = true;
	m_ySpeed = 0.0;
}

Player::~Player() { }

void Player::update(float delta) {
	// Check movement
	m_ySpeed -= G * delta;
	float nextY = m_y + m_ySpeed * delta;
	Cube *next;
	if (m_ySpeed > 0){
		next = m_grid->getCubeAtReal((RealCoords){m_x, nextY + PLAYER_HEIGHT + Y_DIST, m_z});	
	}
	else {
		next = m_grid->getCubeAtReal((RealCoords){m_x, nextY - Y_DIST, m_z});
	}

	if (next == NULL || next->color == EMPTY) {
		m_y = nextY;
	}
	else {
		if (m_ySpeed < 0) {
			if (m_currentLevel >= m_levelReached){
				m_levelReached = m_currentLevel;
			}
			m_lastLevel = m_currentLevel;
		}
		m_ySpeed = 0;
		m_jumping = false;
	}

	if (m_movingForward) {
		moveWithDirection(1, delta);
	}

	else if (m_movingBackwards) {
		moveWithDirection(-1, delta);
	}

	// Level checking
	m_currentLevel = m_y / m_grid->getCubeSize();

	// Raycast selection
	if (m_chosen) {
		m_chosen->isChosen = false;
	}

	RealCoords start {m_x, m_y + 0.9f, m_z};
	RealCoords current {m_x, m_y + 0.9f, m_z};
	Cube *cb;
	
	while (distanceRealCoords(start, current) <= RAY_LENGTH) {
		current.x -= 0.1f * sin(m_angle);
		current.y += 0.1f * sin(m_pitch);
		current.z -= 0.1f * cos(m_angle);
		cb = m_grid->getCubeAtReal(current);
		if (cb && cb->color != EMPTY) {
			cb->isChosen = true;
			m_chosen = cb;
			break;
		}
	}

	// Flashlight position
	GLfloat lightPos[] = {
			m_x - 0.25f * sin(m_angle),
			1.0f + m_y + (2 * PLAYER_HEIGHT) / 3.0f + 0.25f * sin(m_pitch),
			m_z - 0.25f * cos(m_angle),
			1.0f
	};

	glLightfv(GL_LIGHT4, GL_POSITION, lightPos);

}

void Player::render() {
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();

	glTranslatef(m_x, m_y + Y_DIST, m_z);
	glRotatef(toDegrees(m_angle) + 180, 0.0, 1.0, 0.0);	
	
	// Legs
	glColor3f(0.25, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0.125, 0.0, 0.0);
	glScalef(1.0, 1.5, 1.0);
	glutSolidCube(0.15);
	glTranslatef(-0.25, 0.0, 0.0);
	glutSolidCube(0.15);
	glPopMatrix();
	
	// Body
	glColor3f(0.5, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0.0, 0.15 * 1.5 + 0.1, 0.0);
	glPushMatrix();
	glScalef(1.6, 1.8, 1.6);
	glutSolidCube(0.25);
	glPopMatrix();

	//Hands
	glColor3f(0.75, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(-0.25, 0.0, 0.25);
	glScalef(1.0, 1.0, 2.5);
	glutSolidCube(0.125);
	glTranslatef(0.5, 0.0, 0.0);
	glutSolidCube(0.15);
	glPopMatrix();
	glPopMatrix();


	// Head
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0.0, 0.7, 0.0);
	glutSolidSphere(0.15, 25, 25);
	glPopMatrix();
	glPopMatrix();
}

bool Player::canMoveTo(RealCoords coords) {
	GreedyCoords g = m_grid->transformRealToGreedy(coords);
	DEBUG(cout << "player has (x: " << coords.x << ", y: " << coords.y << ", z: " << coords.z << ")" << endl);

	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			GreedyCoords gt = g;
			gt.x += i;
			gt.z += j;

			if (m_grid->doCollide(coords, gt)) {
				DEBUG(cout << "collision with (x: " << gt.x << ", y: " << gt.y << ", z: " << gt.z << ")" << endl);
				return false;
			}
		}
	}
	return true;
}

void Player::moveTo(RealCoords coords) {
	m_x = coords.x;
	m_y = coords.y;
	m_z = coords.z;
}

void Player::turnLeft() {
	m_angle += MOVE_ANGLE;
	DEBUG(cout << "new angle " << m_angle << endl);
}

void Player::turnRight() {
	m_angle -= MOVE_ANGLE;
	DEBUG(cout << "new angle " << m_angle << endl);
}

void Player::moveWithDirection(int direction, float delta) {
	// 1 for front, -1 for back
	assert(direction == 1 || direction == -1);
	RealCoords candidate = (RealCoords){
		m_x - direction * (MOVE_SPEED * sin(m_angle)) * delta,
		m_y,
		m_z - direction * (MOVE_SPEED * cos(m_angle)) * delta
	};

	if (canMoveTo(candidate)) {
		moveTo(candidate);
	}
}

void Player::moveForward() {
	m_movingForward = true;

	DEBUG(cout << "moved to (x:" << m_x << ", z:" << m_z << ")" << endl);
	DEBUG(cout << "with angle " << m_angle << endl);
}

void Player::stopMovingForward() {
	m_movingForward = false;
}

void Player::moveBackwards() {
	m_movingBackwards = true;

	DEBUG(cout << "moved to (x:" << m_x << ", z:" << m_z << ")" << endl);
	DEBUG(cout << "with angle " << m_angle << endl);
}

void Player::stopMovingBackwards() {
	m_movingBackwards = false;
}

void Player::lookUp() {
	m_pitch -= PITCH_DOWN_LIMIT >= m_pitch ? 0 : MOVE_ANGLE;
}

void Player::lookDown() {
	m_pitch += PITCH_UP_LIMIT <= m_pitch ? 0 : MOVE_ANGLE;
}

void Player::toggleCameraView(){
	m_firstPerson = !m_firstPerson;
}

void Player::jump() {
	if (!m_jumping && m_ySpeed == 0){
		m_ySpeed += JUMP_SPEED;
	}
}

void Player::pickUpCube() {
	if (m_chosen && m_chosen->color != MAGENTA) {
		m_reserve.push_back(Cube(m_chosen->color));
		m_chosen->color = EMPTY;
	}
}

void Player::placeCube() {
	if (m_reserve.size() > 0) {
		Cube *front = m_grid->getCubeAtReal((RealCoords){ 
				m_x - PLACE_DISTANCE * sin(m_angle), 
				m_y + PLAYER_HEIGHT + PLACE_DISTANCE * sin(m_pitch),
				m_z - PLACE_DISTANCE * cos(m_angle)
				});
		if (front && front->color == EMPTY) {
			front->color = m_reserve.back().color;
			m_reserve.pop_back();
		}
	}
}

void Player::updateView() {
	if (!m_firstPerson) {
		gluLookAt(
			-(TP_RADIUS * sin(M_PI + m_angle)) + m_x,
			m_y + 2.0,
			-(TP_RADIUS * cos(M_PI + m_angle)) + m_z,

			m_x,
			m_y + 1.0 + m_pitch,
			m_z,

			0.0,
			1.0,
			0.0
		);
	}
	else {
		gluLookAt(
			(FP_RADIUS * sin(M_PI + m_angle)) + m_x,
			m_y + 0.9,
			(FP_RADIUS * cos(M_PI + m_angle)) + m_z,

			m_x - 1.0 * sin(m_angle),
			m_y + 1.0 + m_pitch,
			m_z - 1.0 * cos(m_angle),

			0.0,
			1.0,
			0.0
		);
	}
}

char* Player::getHUDString() {
	char *buffer = new char[512];

	sprintf(buffer,
		"Level: %d\n"
		"Max level reached: %d\n"
		"Meaning of life: %d\n"
		, m_currentLevel, m_levelReached, 42);
	return buffer;
}
