#include <iostream>
#include <cmath>

#include "player.h"
#include "debug.h"
#define _USE_MATH_DEFINES

using namespace std;

float toDegrees(float rads){
	return rads * 180 / M_PI;
}

Player::Player(float x, float y, float z) {
	m_x = x;
	m_y = y;
	m_z = z;
	m_angle = 0.0;
	m_radius = 2.0;
}

Player::~Player() { }

void Player::update(float delta) { }

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
}

void Player::turnLeft(){
	m_angle += 0.1;
	DEBUG(cout << "new angle " << m_angle << endl);
}

void Player::turnRight(){
	m_angle -= 0.1;
	DEBUG(cout << "new angle " << m_angle << endl);
}

void Player::moveForward() {
	m_z -= 0.1 * cos(m_angle);
	m_x -= 0.1 * sin(m_angle);

	DEBUG(cout << "moved to (x:" << m_x << ", z:" << m_z << ")" << endl);
	DEBUG(cout << "with angle " << m_angle << endl);
}

void Player::moveBackwards(){
	m_z += 0.1 * cos(m_angle);
	m_x += 0.1 * sin(m_angle);

	DEBUG(cout << "moved to (x:" << m_x << ", z:" << m_z << ")" << endl);
	DEBUG(cout << "with angle " << m_angle << endl);
}

void Player::updateView(){
	gluLookAt(
		-(m_radius * sin(M_PI + m_angle)) + m_x,
		m_y + 2.0,
		-(m_radius * cos(M_PI + m_angle)) + m_z,

		m_x,
		m_y + 1.0,
		m_z,

		0.0,
		1.0,
		0.0
	);
}
