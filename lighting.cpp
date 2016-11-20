#include "lighting.h"

const GLfloat lightSpec0[] = {1.0, 1.0, 1.0, 1.0};
const GLfloat lightDif0[] = {1.0, 1.0, 1.0, 1.0};

bool light0 = true;
bool light1 = true;
bool light2 = true;
bool light3 = true;

void initLighting(int gridSize, float tileSize) {
	GLfloat lightPos0[] = {-1.0f, (gridSize + 1.0f) * tileSize, -1.0f, 0.0f};
	GLfloat lightPos1[] = {-1.0f, (gridSize + 1.0f) * tileSize, (gridSize + 1.0f) * tileSize, 0.0f};
	GLfloat lightPos2[] = {(gridSize + 1.0f) * tileSize, (gridSize + 1.0f) * tileSize, -1.0f, 0.0f};
	GLfloat lightPos3[] = {(gridSize + 1.0f) * tileSize, (gridSize + 1.0f) * tileSize, (gridSize + 1.0f) * tileSize, 0.0f};
	
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
	}
}
