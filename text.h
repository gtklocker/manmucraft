#pragma once
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

const int BUFFER_SIZE = 8192;

class Text {
protected:
	char m_text[BUFFER_SIZE];
	int m_w, m_h;
	float m_paddingLeft, m_paddingTop;
	float m_opacity;

	void renderProlog();
	void renderEpilog();
public:
	Text(int w, int h);
	Text(char *text, int w, int h);
	~Text();

	void onResolutionChange(int w, int h);
	void onTextChange(char *text);
	void update(float delta);
	void render();
};

class TextScreen: public Text {
private:
	int m_longestLine;
	int m_numLines;
	float m_animationTime;
	int m_animationDirection;

	void recalculateMargins();
public:
	using Text::Text;
	TextScreen(char *text, int w, int h);

	void onTextChange(char *text);
	void onResolutionChange(int w, int h);

	void reset();
	void show();
	void hide();

	bool update(float delta);
	void render();
};
