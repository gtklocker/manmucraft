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
const float CAMERA_CHANGE_SPEED = 0.75;
const float TP_RADIUS = 1.5;
const float FP_RADIUS = 0.085;
const float RAY_LENGTH = 2.0;
const float PITCH_UP_LIMIT = 1.5;
const float PITCH_DOWN_LIMIT = -3.0;
const float Y_DIST = .125;
const float JUMP_SPEED = 3.0;
const float PLAYER_HEIGHT = 0.9;
const float G = 9.807 / 5;
const float PLACE_DISTANCE = 1.75f;
const int STARTING_LIVES = 3;
const int STARTING_POINTS = 50;
const float ANIMATION_TIME = 1.25f;

Player::Player(float x, float y, float z, Grid *grid) {
	m_startX = x;
	m_startY = y;
	m_startZ = z;
	m_grid = grid;

	m_angle = 0.0;
	m_firstPerson = true;
	m_cameraRadius = FP_RADIUS;
	m_ySpeed = 0.0;
	m_points = STARTING_POINTS;
	m_lives = STARTING_LIVES;
	
	respawn();
	m_animationTimer = 0.0f;
}

Player::~Player() { }

void Player::update(float delta) {
	if (m_firstPerson) {
		m_cameraRadius -= CAMERA_CHANGE_SPEED * delta;
		if (m_cameraRadius < FP_RADIUS) {
			m_cameraRadius = FP_RADIUS;
		}
	}
	else {
		m_cameraRadius += CAMERA_CHANGE_SPEED * delta;
		if (m_cameraRadius > TP_RADIUS) {
			m_cameraRadius = TP_RADIUS;
		}
	}

	m_cameraShake += 0.075 * delta;
	if (!m_firstPerson || (!m_movingForward && !m_movingBackwards)) {
		m_cameraShake = 0;
	}

	// Level checking
	m_currentLevel = m_y / m_grid->getCubeSize();
	if (m_currentLevel < -2) { // -2, let him fall a bit
		death();
	}

	// Check movement
	m_ySpeed -= G * delta;
	float nextY = m_y + m_ySpeed * delta;

	RealCoords nextCoords;
	if (m_ySpeed > 0){
		nextCoords = (RealCoords){m_x, nextY + PLAYER_HEIGHT + Y_DIST, m_z};
	}
	else {
		nextCoords = (RealCoords){m_x, nextY - Y_DIST, m_z};
	}

	if (canMoveTo(nextCoords)) {
		m_y = nextY;
	}
	else {
		if (m_ySpeed < 0) {
			if (m_currentLevel > m_levelReached) {
				m_points += (m_currentLevel - m_levelReached) * 5;
				m_levelReached = m_currentLevel;
				if (m_levelReached == m_grid->getGridSize() - 1) {
					m_lives++;
					m_points += 100;
				}
			}
			else if (m_lastLevel - m_currentLevel > 1) {
				m_points -= (m_lastLevel - (m_currentLevel + 1)) * 5;
				if (m_points < 0) {
					m_points = STARTING_POINTS;
					death();
				}
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
			m_chosenCoords = current;

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

	if (m_movingForward) {
		m_animationTimer += delta;
	}
	else {
		m_animationTimer -= delta;
	}

	if (!m_movingForward && !m_movingBackwards) {
		m_animationTimer = 0;
	}
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
	glPushMatrix();
	glRotatef(50 * sin(m_animationTimer / ANIMATION_TIME), 1.0, 0.0, 0.0);
	glutSolidCube(0.15);
	glPopMatrix();
	glTranslatef(-0.25, 0.0, 0.0);
	glPushMatrix();
	glRotatef(-50 * sin(m_animationTimer / ANIMATION_TIME), 1.0, 0.0, 0.0);
	glutSolidCube(0.15);
	glPopMatrix();
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
	glTranslatef(-0.25, 0.0, 0.0);
	glScalef(1.0, 2.5, 1.0);
	glPushMatrix();
	glRotatef(50 * sin(m_animationTimer / ANIMATION_TIME), 1.0, 0.0, 0.0);
	glutSolidCube(0.125);
	glPopMatrix();
	glTranslatef(0.5, 0.0, 0.0);
	glPushMatrix();
	glRotatef(-50 * sin(m_animationTimer / ANIMATION_TIME), 1.0, 0.0, 0.0);
	glutSolidCube(0.15);
	glPopMatrix();
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
		m_points -= 5;
		if (m_points < 0) {
			m_points = STARTING_POINTS;
			death();
		}
	}
}

void Player::placeCube() {
	if (m_reserve.size() > 0) {
		RealCoords pos = { 
				m_x - PLACE_DISTANCE * sin(m_angle), 
				m_y + PLAYER_HEIGHT + PLACE_DISTANCE * sin(m_pitch),
				m_z - PLACE_DISTANCE * cos(m_angle)
				};

		if (m_grid->placeCube(pos, m_reserve.back().color)) {
			m_reserve.pop_back();
			m_points += 5;
		}
	}
}

void Player::removeColumn() {
	RealCoords crds = {m_x - (PLACE_DISTANCE) * sin(m_angle),
			0,
			m_z - (PLACE_DISTANCE) * cos(m_angle)
	};

	if (m_grid->getCubeAtReal(crds)) {
		m_grid->removeColumn(crds);
		m_lives++;
		m_points -= 20;
		if (m_points < 0) {
			m_points = STARTING_POINTS;
			death();
		}
	}
}

void Player::respawn() {
	m_x = m_startX;
	m_y = m_startY;
	m_z = m_startZ;
	m_currentLevel = 0;
}

void Player::death() {
	m_lives--;
	respawn();
	//TODO:Death screen if lives < 0
}

void Player::updateView() {
	gluLookAt(
		((m_cameraRadius * sin(M_PI + m_angle))) 
			- (1.5 * (m_cameraRadius * sin(M_PI + m_angle))) 
			* ((m_cameraRadius - FP_RADIUS / (TP_RADIUS - FP_RADIUS))) + m_x,
		m_y  + 0.015 * sin(m_cameraShake / 0.015) + 0.9 
		+ 1.1 * ((m_cameraRadius - FP_RADIUS) / (TP_RADIUS - FP_RADIUS)),
		(m_cameraRadius * cos(M_PI + m_angle))
			- (1.5 * (m_cameraRadius * cos(M_PI + m_angle))) 
			* ((m_cameraRadius - FP_RADIUS / (TP_RADIUS - FP_RADIUS))) + m_z,

		m_x - 1.0 * sin(m_angle),
		m_y + 1.0 + m_pitch,
		m_z - 1.0 * cos(m_angle),

		0.0,
		1.0,
		0.0
	);
}

char* Player::getHUDString() {
	char *buffer = new char[512];

	sprintf(buffer,
		"Level: %d\n"
		"Max level reached: %d\n"
		"Cubes: %d\n"
		"Points: %d\n"
		"Lives: %d\n"
		, m_currentLevel, m_levelReached, (int)m_reserve.size(), m_points, m_lives);
	return buffer;
}

void Player::kickCube() {
	GreedyCoords current = m_grid->transformRealToGreedy((RealCoords){m_x, m_y + 0.5f, m_z});
	GreedyCoords chosen = m_grid->transformRealToGreedy(m_chosenCoords);

	if (current.y != chosen.y) {
		return;
	}

	m_grid->kickCube(chosen, (GreedyCoords){current.x - chosen.x, 0, current.z - chosen.z});
}
