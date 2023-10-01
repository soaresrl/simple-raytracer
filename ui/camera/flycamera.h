#ifndef FLYCAMERA_H
#define FLYCAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

const double PI = 3.14159265;

class FlyCamera {
public:
	FlyCamera(int _windowWidth, int _windowHeight);

	void moveForward(float amount);
	void moveRight(float amount);
	void moveUp(float amount);
	void look(float yaw, float pitch);
	void setAspectRatio(int height, int width);
	QMatrix4x4 getViewProjection();
	QMatrix4x4 getView();

	QVector3D position{ 1.0f, 0.0f, 1.0f };
private:
	float speed = 0.2f;
	float mouseSensitivity = 0.5f;
	QVector3D direction{ 1.0f, -1.0f, -1.0f };
	QVector3D up{ 0.0f, 1.0f, 0.0f };

	float fov = 45.0f;
	float m_near = 0.1f;
	float m_far = 1000.0f;

	int windowHeight;
	int windowWidth;
	float aspectRatio;
};

#endif // !FLYCAMERA_H