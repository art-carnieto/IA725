#include "Scene.hpp"

Scene::Scene() { }

Scene::Scene(Vector3f camera_pos, float fov, float ar, float nearZ, float farZ, bool perspective) {
	this->camera = Camera(camera_pos, fov);
	this->ar = ar;
	this->nearZ = nearZ;
	this->farZ = farZ;
	this->perspective = perspective;
	this->World;
	this->WVP;
}

void Scene::pushMesh(Mesh m) {
	this->meshes.emplace_back(m);
}

Mesh Scene::getMesh(int index) {
	return this->meshes[index];
}

void Scene::clearMeshes() {
	this->meshes.clear();
}

void Scene::setCamera(Camera cam) {
	this->camera = cam;
}

Camera Scene::getCamera() {
	return this->camera;
}

Transformation Scene::getWorldTransformation() {
	return this->World;
}

void Scene::setWorldTransformation(Transformation t) {
	this->World = t;
}

Transformation Scene::getWVPTransformation() {
	return this->WVP;
}

void Scene::setWVPTransformation(Transformation t) {
	this->WVP = t;
}

void Scene::setAR(float ar) {
	this->ar = ar;
}

void Scene::setAR(float width, float height) {
	this->ar = width / height;
}

float Scene::getAR() {
	return this->ar;
}

void Scene::setClippingPlanes(float nearZ, float farZ) {
	this->nearZ = nearZ;
	this->farZ = farZ;
}

float* Scene::getClippingPlanes() {
	float clipping_planes[] = { this->nearZ, this->farZ };
	return clipping_planes;
}

void Scene::setPerspecive() {
	this->perspective = true;
}

void Scene::setOrthogonal() {
	this->perspective = false;
}

bool Scene::isPerspective() {
	return this->perspective;
}