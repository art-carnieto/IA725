#include "Camera.hpp"

Camera::Camera() { }

Camera::Camera(Vector3f pos, float fov) {
	this->t_cam;
	this->cameraPos = pos;
	this->fov = fov;
}

void Camera::setCameraPos(Vector3f new_pos) {
	this->cameraPos = new_pos;
}

Vector3f Camera::getCameraPos() {
	return this->cameraPos;
}

void Camera::setFOV(float new_fov) {
	this->fov = new_fov;
}

float Camera::getFOV() {
	return this->fov;
}

Matrix4f Camera::getFinalTransformation() {
	this->t_cam.setTranslation({
		-(this->cameraPos[0]),
		-(this->cameraPos[1]),
		-(this->cameraPos[2])
	});
	this->t_cam.calcFinalTransformation();

	return this->t_cam.getFinalTransformation();
}

void Camera::moveUp(float amount) {
	setCameraPos({
		this->cameraPos[0],
		this->cameraPos[1] + amount,
		this->cameraPos[2]
		});
}

void Camera::moveDown(float amount) {
	setCameraPos({
		this->cameraPos[0],
		this->cameraPos[1] - amount,
		this->cameraPos[2]
		});
}

void Camera::moveLeft(float amount) {
	setCameraPos({
		this->cameraPos[0] - amount,
		this->cameraPos[1],
		this->cameraPos[2]
		});
}

void Camera::moveRight(float amount) {
	setCameraPos({
		this->cameraPos[0] + amount,
		this->cameraPos[1],
		this->cameraPos[2]
		});
}

void Camera::moveFront(float amount) {
	setCameraPos({
		this->cameraPos[0],
		this->cameraPos[1],
		this->cameraPos[2] + amount
		});
}

void Camera::moveBack(float amount) {
	setCameraPos({
		this->cameraPos[0],
		this->cameraPos[1],
		this->cameraPos[2] - amount
		});
}