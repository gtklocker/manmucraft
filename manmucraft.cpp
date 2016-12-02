#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "grid.h"
#include "player.h"
#include "lighting.h"
#include "text.h"

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
int currentWindowWidth;
int currentWindowHeight;

const float STEP = 60.0 / 1000.0; // 60 ticks per 1000 ms

const int DEFAULT_GRID_SIZE = 10;
const int TILE_SIZE = 1.0;

int GRID_SIZE = DEFAULT_GRID_SIZE;

int timer;
int oldTime;
int newTime;
float delta;

int frames;
int ticks;

bool isFullscreen = false;

float lastX;
float lastY;

Grid *grid;
Player *player;

Text HUD(WINDOW_WIDTH, WINDOW_HEIGHT);

char *helpText = new char[2048];
TextScreen helpScreen(
	""
	"Welcome to manmucraft! You can use the mouse to look around.\n"
	"\n"
	"Here are the keys you can use to play:\n"
	"w           move forward\n"
	"s           move backwards\n"
	"a           look left\n"
	"d           look right\n"
	"Space       jump\n"
	"\n"
	"q           kick cubes\n"
	"e           remove a column\n"
	"r           drop cubes\n"
	"p           respawn\n"
	"f           toggle the flashlight\n"
	"1-4         toggle the lights\n"
	"\n"
	"v           switch between first and third person mode\n"
	"F2          fullscreen\n"
	"?           close this menu\n"
	"Esc         quit\n"
	"\n"
	"You can also use the following mouse keys:\n"
	"Left click  pick up selected cube\n"
	"Right click place the last cube you picked\n"
	"\n"
	"Be careful what you type, you might 'trip' on an easter egg ;)\n"
	"\n"
	"You can always get back to this menu by pressing ?\n"
	"\n"
	"Go ahead and press ? to start playing.\n", WINDOW_WIDTH, WINDOW_HEIGHT);

bool showHelpScreen = false, renderHelpScreen = false;
bool isPaused = false;

const char *TRIP_STRING = "trip";
int tripIndex = 0;

void initWorld(int gridSize) {
	grid = new Grid(gridSize, TILE_SIZE);
	player = new Player(gridSize / 2, .5, gridSize / 2, grid);
}

void update(float delta) {
	lastX = currentWindowWidth / 2;
	lastY = currentWindowHeight / 2;
	glutWarpPointer(lastX, lastY);

	if (renderHelpScreen) {
		if (helpScreen.update(delta)) {
			return;
		}
		else {
			if (!showHelpScreen) {
				renderHelpScreen = false;
			}
		}
	}

	grid->update(delta);
	player->update(delta);
}

void drawHUD() {
	char *hudText = player->getHUDString();
	HUD.onTextChange(hudText);

	delete[] hudText;

	HUD.render();
}

void draw() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	player->updateView();
	glPushMatrix();	
	grid->render();
	player->render();
	glPopMatrix();

	if (!renderHelpScreen) {
		drawHUD();
	}
	else {
		helpScreen.render();
	}
}

// Called on start or when the window dimensions change
void reshapeHandler(int w, int h) {
	currentWindowWidth = w;
	currentWindowHeight = h;

	HUD.onResolutionChange(w, h);
	helpScreen.onResolutionChange(w, h);

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

void toggleHelpScreen() {
	renderHelpScreen = true;
	isPaused = !isPaused;

	if (showHelpScreen) {
		helpScreen.hide();
		showHelpScreen = false;
	}
	else {
		helpScreen.show();
		showHelpScreen = true;
	}
}

void specialKeypressHandler(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_F2:
			toggleFullscreen();
			break;
		default:
			break;
	}
}

void keypressHandler(unsigned char key, int x, int y) {
	if (key == TRIP_STRING[tripIndex]) {
		++tripIndex;
		if (tripIndex >= strlen(TRIP_STRING)) {
			grid->toggleTrippy();
			tripIndex = 0;
		}

		return;
	}
	else {
		tripIndex = 0;
	}

	switch (key) {
		case 27: // Escape
			exit(0);
			break;
		case '?':
		case '/':
			toggleHelpScreen();
			break;
	}

	if (isPaused) {
		return;
	}

	switch (key) {
		case 'v':
			// toggle camera view
			player->toggleCameraView();
			break;
		case ' ':
			player->jump();
			break;
		case 'a':
			player->turnLeft();
			break;
		case 'd':
			player->turnRight();
			break;
		case 'w':
			player->moveForward();
			break;
		case 's':
			player->moveBackwards();
			break;
		case 'q':
			player->kickCube();
			break;
		case 'e':
			player->removeColumn();
			break;
		case 'r':
			grid->dropCubes();
			break;
		case 'f':
			toggleLight(GL_LIGHT4);
			break;
		case 'p':
			player->respawn();
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
		default:
			break;
	}
}

void keyReleaseHandler(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
			player->stopMovingForward();
			break;
		case 's':
			player->stopMovingBackwards();
			break;
	}
}

void mouseMoveHandler(int x, int y) {
	if (isPaused) {
		return;
	}

	if (x > lastX) {
		player->turnRight();
	}
	else if (x < lastX) {
		player->turnLeft();
	}

	if (y > lastY) {
		player->lookUp();
	}
	else if (y < lastY) {
		player->lookDown();
	}
	lastX = x;
	lastY = y;
}

void mouseClickHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		player->pickUpCube();
	}

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		player->placeCube();
	}
}

int main(int argc, char *argv[]) {
	int gridSize = DEFAULT_GRID_SIZE;
	if (argc > 1) {
		sscanf(argv[argc - 1], "%d", &gridSize);
	}

	initWorld(gridSize);

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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
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

	toggleHelpScreen();
	glutMainLoop();
}
