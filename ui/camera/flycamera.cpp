#include "flycamera.h"


FlyCamera::FlyCamera(int _windowWidth, int _windowHeight) {
	windowHeight = _windowHeight;
	windowWidth = _windowWidth;

	aspectRatio = windowWidth / windowHeight;
}

void FlyCamera::moveForward(float amount) {
	position += direction * amount * speed;
}

void FlyCamera::moveRight(float amount) {
	QVector3D right = QVector3D::crossProduct(direction, up);
	position += right * amount * speed;
}

void FlyCamera::moveUp(float amount) {
	//QVector3D right = QVector3D::crossProduct(direction, up);
	position += up * amount * speed;
}


void FlyCamera::look(float yaw, float pitch) {
	yaw *= mouseSensitivity;
	pitch *= mouseSensitivity;

	QVector3D right = QVector3D::crossProduct(direction, up);

	QMatrix4x4 yawRotation;
	QMatrix4x4 pitchRotation;
	yawRotation.setToIdentity();
	pitchRotation.setToIdentity();

	yawRotation.rotate(yaw * PI / 180.0f, up);
	pitchRotation.rotate(pitch * PI / 180.0f, right);

	up = QVector3D(pitchRotation * QVector4D{ up, 0.0f });
	direction = QVector3D(yawRotation * pitchRotation * QVector4D{ direction, 0.0f });
}

QMatrix4x4 FlyCamera::getViewProjection() {
	QMatrix4x4 projection;

	projection.setToIdentity();
	projection.perspective(fov, aspectRatio, m_near, m_far);

	return projection * getView();
}

QMatrix4x4 FlyCamera::getView() {
	QMatrix4x4 identity;
	identity.setToIdentity();
	identity.lookAt(position, position + direction, up);

	return identity;
}

void FlyCamera::setAspectRatio(int height, int width) {
	windowHeight = height;
	windowWidth = width;

	aspectRatio = (float)width / (float)height;
}