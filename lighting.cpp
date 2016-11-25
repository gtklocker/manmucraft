#include "lighting.h"

bool light0 = true;
bool light1 = true;
bool light2 = true;
bool light3 = true;
bool light4 = true;

void initLighting(int gridSize, float tileSize) {
	GLfloat lightPos0[] = {-1.0f, (gridSize + 1.0f) * tileSize, -1.0f, 0.0f};
	GLfloat lightPos1[] = {-1.0f, (gridSize + 1.0f) * tileSize, (gridSize + 1.0f) * tileSize, 0.0f};
	GLfloat lightPos2[] = {(gridSize + 1.0f) * tileSize, (gridSize + 1.0f) * tileSize, -1.0f, 0.0f};
	GLfloat lightPos3[] = {(gridSize + 1.0f) * tileSize, (gridSize + 1.0f) * tileSize, (gridSize + 1.0f) * tileSize, 0.0f};

	GLfloat lightSpec0[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lightDif0[] = {1.0, 1.0, 1.0, 1.0};

	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif0);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpec0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDif0);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpec0);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDif0);
	glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, lightSpec0);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDif0);

	// Flashlight init
	glEnable(GL_LIGHT4);
	GLfloat flashlightSpec[] = {0.50, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT4, GL_SPECULAR, flashlightSpec);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, flashlightSpec);
}

void toggleLight(GLint light) {
	switch(light) {
		case GL_LIGHT0:
			if (light0) {
				glDisable(GL_LIGHT0);
			}
			else {
				glEnable(GL_LIGHT0);
			}
			light0 = !light0;
			break;
		case GL_LIGHT1:
			if (light1) {
				glDisable(GL_LIGHT1);
			}
			else {
				glEnable(GL_LIGHT1);
			}
			light1 = !light1;
			break;
		case GL_LIGHT2:
			if (light2) {
				glDisable(GL_LIGHT2);
			}
			else {
				glEnable(GL_LIGHT2);
			}
			light2 = !light2;
			break;
		case GL_LIGHT3:
			if (light3) {
				glDisable(GL_LIGHT3);
			}
			else {
				glEnable(GL_LIGHT3);
			}
			light3 = !light3;
			break;
		case GL_LIGHT4:
			if (light4) {
				glDisable(GL_LIGHT4);
			}
			else {
				glEnable(GL_LIGHT4);
			}
			light4 = !light4;
			break;
	}
}
