#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "grid.h"
#include "player.h"

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float STEP = 60.0 / 1000.0; // 60 ticks per 1000 ms

const int GRID_SIZE = 10;
const int TILE_SIZE = 1.0;
const float P_START_POS_X = 0.0;
const float P_START_POS_Y = 0.5;
const float P_START_POS_Z = 0.0;

int timer;
int oldTime;
int newTime;
float delta;
int frames;
int ticks;

bool isFullscreen = false;

Grid grid(GRID_SIZE, TILE_SIZE);
Player player(P_START_POS_X, P_START_POS_Y, P_START_POS_Z);

void update(float delta) {
}

void draw() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	player.updateView();
	glPushMatrix();	

	grid.render();
	player.render();

	glPopMatrix();
}

// Called on start or when the window dimensions change
void reshapeHandler(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0.0, 0.0, (GLsizei) w, (GLsizei) h);
	gluPerspective(60, (GLfloat) w / (GLfloat) h, 0.1, 1000);
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
			player.turnLeft();
			break;
		case GLUT_KEY_RIGHT:
			player.turnRight();
			break;
		case GLUT_KEY_UP:
			player.moveForward();
			break;
		case GLUT_KEY_DOWN:
			player.moveBackwards();
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Manmucraft!");

	glEnable(GL_DEPTH_TEST);
	
	glutDisplayFunc(render);
	glutReshapeFunc(reshapeHandler);
	glutIdleFunc(render);
	glutSpecialFunc(specialKeypressHandler);
	glutKeyboardFunc(keypressHandler);
	
	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_VENDOR) << endl;
	cout << glGetString(GL_RENDERER) << endl;

	oldTime = glutGet(GLUT_ELAPSED_TIME);	
	glutMainLoop();
}
