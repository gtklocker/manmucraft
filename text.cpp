#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include "text.h"

using namespace std;

#define BITMAP_FONT GLUT_BITMAP_9_BY_15
const int BITMAP_W = 9, BITMAP_H = 15;

Text::Text(int w, int h) {
	m_w = w;
	m_h = h;
	m_opacity = 1.0;

	m_paddingTop = 0;
	m_paddingLeft = 0;
}

Text::Text(char *text, int w, int h) {
	m_w = w;
	m_h = h;

	m_paddingTop = 0;
	m_paddingLeft = 0;
	strncpy(m_text, text, BUFFER_SIZE);
}

Text::~Text() {
}

void Text::onTextChange(char *text) {
	strncpy(m_text, text, BUFFER_SIZE);
}

void Text::onResolutionChange(int w, int h) {
	m_w = w;
	m_h = h;
}

void Text::update(float delta) {
}

void Text::renderProlog() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, m_w, m_h, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
}

void Text::renderEpilog() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Text::render() {
	renderProlog();

	glColor4f(1.0, 1.0, 1.0, m_opacity);

	int linesBefore = 1;
	glRasterPos2i(m_paddingLeft * m_w + BITMAP_W, m_paddingTop * m_h + BITMAP_H * linesBefore);
	for (int i = 0; i < strlen(m_text); ++i) {
		if (m_text[i] == '\n') {
			glRasterPos2i(m_paddingLeft + BITMAP_W, m_paddingTop + BITMAP_H * ++linesBefore);
		}
		else {
			glutBitmapCharacter(BITMAP_FONT, m_text[i]);
		}
	}

	renderEpilog();
}

TextScreen::TextScreen(char *text, int w, int h) : Text(text, w, h) {
	recalculateMargins();
}

void TextScreen::onTextChange(char *text) {
	Text::onTextChange(text);
	recalculateMargins();
}

void TextScreen::recalculateMargins() {
	m_longestLine = 0;
	m_numLines = 0;
	for (char *oldNextline = m_text, *nextNewline = m_text; nextNewline != NULL; oldNextline = nextNewline, nextNewline = strchr(oldNextline + 1, '\n')) {
		++m_numLines;
		m_longestLine = max(m_longestLine, (int)(nextNewline - oldNextline));
	}

	m_paddingLeft = (m_w - (m_longestLine * BITMAP_W)) / 2;
	m_paddingTop = (m_h - (m_numLines * BITMAP_H)) / 2;
}

void TextScreen::onResolutionChange(int w, int h) {
	Text::onResolutionChange(w, h);
	recalculateMargins();
}

void TextScreen::reset() {
	m_animationTime = .0f;
}

bool TextScreen::update(float delta) {
	if (m_animationTime >= 1) {
		return false;
	}
	m_animationTime += delta * .55;
	m_opacity = m_animationDirection < 0 ? (1 - m_animationTime) : m_animationTime;
	m_opacity = pow(m_opacity, 4);
	m_opacity = m_opacity > 1.0 ? 1.0 : m_opacity;
	return true;
}

void TextScreen::show() {
	reset();
	m_animationDirection = 1;
}

void TextScreen::hide() {
	reset();
	m_animationTime = .0f;
	m_animationDirection = -1;
}

void TextScreen::render() {
	Text::renderProlog();
	glColor4f(0.0, 0.0, 0.0, m_opacity * .8);
	glRecti(0, 0, m_w, m_h);
	Text::renderEpilog();

	Text::render();
}
