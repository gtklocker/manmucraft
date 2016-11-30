#include <iostream>
#include <cstdlib>
#include <cstring>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "grid.h"
#include "player.h"
#include "lighting.h"

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
int currentWindowWidth;
int currentWindowHeight;
const float STEP = 60.0 / 1000.0; // 60 ticks per 1000 ms

const int GRID_SIZE = 10;
const int TILE_SIZE = 1.0;
const float P_START_POS_X = GRID_SIZE / 2;
const float P_START_POS_Y = 0.5;
const float P_START_POS_Z = GRID_SIZE / 2;

#define BITMAP_FONT GLUT_BITMAP_9_BY_15
const int BITMAP_W = 9, BITMAP_H = 15;

int timer;
int oldTime;
int newTime;
float delta;
int frames;
int ticks;

bool isFullscreen = false;

float lastX;
float lastY;

Grid grid(GRID_SIZE, TILE_SIZE);
Player player(P_START_POS_X, P_START_POS_Y, P_START_POS_Z, &grid);

void update(float delta) {
	if (lastX > currentWindowWidth - 100 || lastX < 100
		|| lastY > currentWindowHeight - 100 || lastY < 100){
		lastX = currentWindowWidth / 2;
		lastY = currentWindowHeight / 2;
		glutWarpPointer(lastX, lastY);
	}

	player.update(delta);
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

void drawText(char text[]) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, currentWindowWidth, currentWindowHeight, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	int linesBefore = 0;
	glRasterPos2i(BITMAP_W, BITMAP_H * (linesBefore + 1));
	for (int i = 0; i < strlen(text); ++i) {
		if (text[i] == '\n') {
			glRasterPos2i(BITMAP_W, BITMAP_H * (++linesBefore + 1));
		}
		else {
			glutBitmapCharacter(BITMAP_FONT, text[i]);
		}
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

// Called on start or when the window dimensions change
void reshapeHandler(int w, int h) {
	currentWindowWidth = w;
	currentWindowHeight = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0.0, 0.0, (GLsizei) w, (GLsizei) h);
	gluPerspective(60, (GLfloat) w / (GLfloat) h, 0.1, 1000);
}

void drawHUD() {
	char *hud = player.getHUDString();

	drawText(hud);

	delete[] hud;
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
	drawHUD();

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
		case GLUT_KEY_F1:
			player.respawn();
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
		case 'q':
			// quit the game
			exit(0);
			break;
		case 'f':
			// toggle fullscreen
			toggleFullscreen();
			break;
		case 'v':
			// toggle camera view
			player.toggleCameraView();
			break;
		case ' ':
			player.jump();
			break;
		case 'a':
			player.turnLeft();
			break;
		case 'd':
			player.turnRight();
			break;
		case 'w':
			player.moveForward();
			break;
		case 's':
			player.moveBackwards();
			break;
		case 'k':
			player.kickCube();
			break;
		case 'e':
			player.removeColumn();
			break;
		case '1':
			toggleLight(GL_LIGHT0);
			break;
		case '2':
			toggleLight(GL_LIGHT1);
			break;
		case '3':
			toggleLight(GL_LIGHT2);
			break;
		case '4':
			toggleLight(GL_LIGHT3);
			break;
		case '5':
			toggleLight(GL_LIGHT4);
			break;
		default:
			break;
	}
}

void keyReleaseHandler(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
			player.stopMovingForward();
			break;
		case 's':
			player.stopMovingBackwards();
			break;
	}
}

void mouseMoveHandler(int x, int y) {
	if (x > lastX) {
		player.turnRight();
	}
	else if (x < lastX) {
		player.turnLeft();
	}

	if (y > lastY) {
		player.lookUp();
	}
	else if (y < lastY) {
		player.lookDown();
	}
	lastX = x;
	lastY = y;
}

void mouseClickHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		player.pickUpCube();
	}

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		player.placeCube();
	}
}

int main(int argc, char *argv[]) {
	timer = 0;
	newTime = 0;
	delta = 0.0;
	frames = 0;
	ticks = 0;

	lastX = WINDOW_WIDTH / 2;
	lastY = WINDOW_HEIGHT / 2;
	
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
	glutKeyboardUpFunc(keyReleaseHandler);
	glutMouseFunc(mouseClickHandler);
	glutPassiveMotionFunc(mouseMoveHandler);
	glutSetCursor(GLUT_CURSOR_NONE);

	initLighting(GRID_SIZE, TILE_SIZE);

	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_VENDOR) << endl;
	cout << glGetString(GL_RENDERER) << endl;

	oldTime = glutGet(GLUT_ELAPSED_TIME);	
	glutWarpPointer(lastX, lastY);

	glutMainLoop();
}
