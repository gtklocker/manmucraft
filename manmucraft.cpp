#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;

const float WINDOW_WIDTH = 500.0;
const float WINDOW_HEIGHT = 500.0;
const float STEP = 60.0 / 1000.0; // 60 ticks per 1000 ms

int timer;
int oldTime;
int newTime;
float delta;
int frames;
int ticks;

void update(float delta) {
}

void draw() {
	glPushMatrix();	
	// Render
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

int main(int argc, char *argv[]) {
	timer = 0;
	newTime = 0;
	delta = 0.0;
	frames = 0;
	ticks = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_HEIGHT, WINDOW_WIDTH);
	glutCreateWindow("Manmucraft!");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutDisplayFunc(render);
	glutIdleFunc(render);

	glEnable(GL_DEPTH_TEST);
	
	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_VENDOR) << endl;
	cout << glGetString(GL_RENDERER) << endl;

	oldTime = glutGet(GLUT_ELAPSED_TIME);	
	glutMainLoop();
}
