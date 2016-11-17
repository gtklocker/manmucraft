#include "player.h"

Player::Player(float x, float y, float z) {
	m_x = x;
	m_y = y;
	m_z = z;
}

Player::~Player() { }

void Player::update(float delta) { }

void Player::render() {
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0.0, 1.0, 0.0);
	glutWireCube(1.0);
	glPopMatrix();
	
	glTranslatef(m_x, m_y + 0.125, m_z);
	
	// Legs
	glColor3f(0.0, 1.0, 0.25);
	glPushMatrix();
	glTranslatef(0.125, 0.0, 0.0);
	glScalef(1.0, 1.5, 1.0);
	glutSolidCube(0.15);
	glTranslatef(-0.25, 0.0, 0.0);
	glutSolidCube(0.15);
	glPopMatrix();
	
	// Body
	glColor3f(0.0, 1.0, 0.5);
	glPushMatrix();
	glTranslatef(0.0, 0.15 * 1.5 + 0.1, 0.0);
	glPushMatrix();
	glScalef(1.6, 1.8, 1.6);
	glutSolidCube(0.25);
	glPopMatrix();

	//Hands
	glColor3f(0.0, 1.0, 0.75);
	glPushMatrix();
	glTranslatef(-0.25, 0.0, 0.25);
	glScalef(1.0, 1.0, 2.5);
	glutSolidCube(0.125);
	glTranslatef(0.5, 0.0, 0.0);
	glutSolidCube(0.15);
	glPopMatrix();
	glPopMatrix();


	// Head
	glColor3f(0.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.7, 0.0);
	glutSolidSphere(0.15, 25, 25);
	glPopMatrix();
	
	glPopMatrix();
}
