#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "grid.h"
#include "camera.h"

using namespace std;

const float WINDOW_WIDTH = 800.0;
const float WINDOW_HEIGHT = 600.0;
const float STEP = 60.0 / 1000.0; // 60 ticks per 1000 ms

const int GRID_SIZE = 10;
const int TILE_SIZE = 1.0;
const float START_POS_X = 0.0;
const float START_POS_Z = 0.1;
const float CAMERA_SPEED = 0.1;

int timer;
int oldTime;
int newTime;
float delta;
int frames;
int ticks;

bool isFullscreen = false;

Grid grid(GRID_SIZE, TILE_SIZE);
Camera camera(START_POS_X, START_POS_Z, CAMERA_SPEED);

void update(float delta) {
}

void draw() {
	glLoadIdentity();
	camera.updateView();
	glPushMatrix();	

	grid.render();

	glPopMatrix();
}

void render() {
	newTime = glutGet(GLUT_ELAPSED_TIME);
	delta += (newTime - oldTime) * STEP;
	oldTime = newTime;
	
	while (delta >= 1) {
		// tick game's logic
		update(STEP);	
		ticks++;
		delta--;
	}
	// render game
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();	
	glutSwapBuffers();
	frames++;

	// Show fps and tps every second
	if (glutGet(GLUT_ELAPSED_TIME) >= timer + 1000) {
		cout << "FPS :" << frames << " TPS :" << ticks << endl;
		frames = 0;
		ticks = 0;
		timer += 1000;
	}
}

void specialKeypressHandler(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			camera.lookLeft();
			break;
		case GLUT_KEY_RIGHT:
			camera.lookRight();
			break;
		case GLUT_KEY_UP:
			camera.moveForward();
			break;
		case GLUT_KEY_DOWN:
			camera.moveBackwards();
			break;
		default:
			break;
	}
}

void toggleFullscreen() {
	if (isFullscreen) {
		glutPositionWindow(0, 0);
		glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else {
		glutFullScreen();
	}
	isFullscreen = !isFullscreen;
}

void keypressHandler(unsigned char key, int x, int y) {
	switch (key) {
		case 'f':
			// toggle fullscreen
			toggleFullscreen();
			break;
		default:
			break;
	}
}

int main(int argc, char *argv[]) {
	timer = 0;
	newTime = 0;
	delta = 0.0;
	frames = 0;
	ticks = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Manmucraft!");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0.0, 0.0, WINDOW_WIDTH, WINDOW_HEIGHT);
	gluPerspective(60, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	
	glutDisplayFunc(render);
	glutIdleFunc(render);
	glutSpecialFunc(specialKeypressHandler);
	glutKeyboardFunc(keypressHandler);
	
	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_VENDOR) << endl;
	cout << glGetString(GL_RENDERER) << endl;

	oldTime = glutGet(GLUT_ELAPSED_TIME);	
	glutMainLoop();
}
