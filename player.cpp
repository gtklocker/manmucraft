#include <iostream>
#include <cmath>
#include <cassert>

#include "player.h"
#include "grid.h"
#include "debug.h"
#define _USE_MATH_DEFINES

using namespace std;

float toDegrees(float rads) {
	return rads * 180 / M_PI;
}

float distanceRealCoords(RealCoords &r1, RealCoords &r2) {
	return sqrt(pow(r1.x - r2.x, 2) + pow(r1.y - r2.y, 2) + pow(r1.z - r2.z, 2));
}

const float MOVE_ANGLE = .05;
const float TP_RADIUS = 1.5;
const float FP_RADIUS = 0.50;
const float RAY_LENGTH = 2.0;

Player::Player(float x, float y, float z, Grid *grid) {
	m_x = x;
	m_y = y;
	m_z = z;
	m_grid = grid;

	m_angle = 0.0;
	m_firstPerson = true;
}

Player::~Player() { }

void Player::update(float delta) {
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
		if (cb && cb->type != EMPTY) {
			cb->isChosen = true;
			m_chosen = cb;
			break;
		}
	}
}

void Player::render() {
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();

	glTranslatef(m_x, m_y + 0.125, m_z);
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
	Cube *currentCube = m_grid->getCubeAtReal((RealCoords){m_x, m_y + 1, m_z});
	Cube *nextCeilCube = m_grid->getCubeAtReal((RealCoords){ceil(coords.x), coords.y + 1, ceil(coords.z)});
	Cube *nextFloorCube = m_grid->getCubeAtReal((RealCoords){floor(coords.x), coords.y + 1, floor(coords.z)});

	if ((nextFloorCube == NULL || nextFloorCube->type == EMPTY) &&
		(nextCeilCube == NULL || nextCeilCube->type == EMPTY)) {
		return true;
	}
	return false;
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

void Player::moveWithDirection(int direction) {
	// 1 for front, -1 for back
	assert(direction == 1 || direction == -1);
	RealCoords candidate = (RealCoords){
		m_x - direction * (0.1f * sin(m_angle)),
		m_y,
		m_z - direction * (0.1f * cos(m_angle))
	};

	if (canMoveTo(candidate)) {
		moveTo(candidate);
	}
}

void Player::moveForward() {
	moveWithDirection(1);

	DEBUG(cout << "moved to (x:" << m_x << ", z:" << m_z << ")" << endl);
	DEBUG(cout << "with angle " << m_angle << endl);
}

void Player::moveBackwards() {
	moveWithDirection(-1);

	DEBUG(cout << "moved to (x:" << m_x << ", z:" << m_z << ")" << endl);
	DEBUG(cout << "with angle " << m_angle << endl);
}

void Player::lookUp() {
	m_pitch -= MOVE_ANGLE;
}

void Player::lookDown() {
	m_pitch += MOVE_ANGLE;
}

void Player::toggleCameraView(){
	m_firstPerson = !m_firstPerson;
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
