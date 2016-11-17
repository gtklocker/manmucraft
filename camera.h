#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

class Camera {
private:
	float m_x, m_z, m_lx, m_lz, m_angle, m_speed;
public:
	Camera(float startX, float startZ, float speed);
	~Camera();
	void moveForward();
	void moveBackwards();
	void lookLeft();
	void lookRight();
	void updateView();
};
