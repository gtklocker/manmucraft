#include "camera.h"

Camera::Camera(float startX, float startZ, float speed) {
	m_x = startX;
	m_z = startZ;
	m_lx = 0.0;
	m_lz = -1.0;
	m_angle = 0.0;
	m_speed = speed;
}

Camera::~Camera() { }

void Camera::updateView() {
	gluLookAt(m_x, 1.0, m_z, m_x + m_lx, 1.0, m_z + m_lz, 0.0, 1.0, 0.0);
}

void Camera::moveForward() {
	m_x += m_lx * m_speed;
	m_z += m_lz * m_speed;
}

void Camera::moveBackwards() {
	m_x -= m_lx * m_speed;
	m_z -= m_lz * m_speed;
}

void Camera::lookLeft() {
	m_angle -= 0.01;
	m_lx = sin(m_angle);
	m_lz = -cos(m_angle);
}

void Camera::lookRight() {
	m_angle += 0.01;
	m_lx = sin(m_angle);
	m_lz = -cos(m_angle);
}
