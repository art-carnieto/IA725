#include "Scene.hpp"

Scene::Scene() { }

Scene::Scene(Vector3f camera_pos, PersProjInfo pers_info, OrthoProjInfo ortho_info, bool perspective) {
	this->camera = Camera(camera_pos, pers_info.FOV);
	this->perspective = perspective;
	this->World;
	this->WVP;
	this->pers_info = pers_info;
	this->ortho_info = ortho_info;
	this->Projection.SetZero();
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

Matrix4f Scene::getWVP() {
	calcWVPTransformation();
	return this->WVP;
}

void Scene::calcWVPTransformation() {
	if (this->perspective == true) {
		this->Projection.InitPersProjTransform(this->pers_info);
	}
	else {
		this->Projection.InitOrthoProjTransform(this->ortho_info);
	}
	this->WVP = this->Projection * this->getCamera().getFinalTransformation() * this->World.getFinalTransformation();
}

void Scene::setClippingPlanes(float nearZ, float farZ) {
	this->pers_info.zNear = nearZ;
	this->pers_info.zFar = farZ;
}

float* Scene::getClippingPlanes() {
	float clipping_planes[] = { this->pers_info.zNear, this->pers_info.zFar };
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