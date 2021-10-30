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

void Scene::genVBO(int index, GLuint* VBO) {
	getMesh(index).genVBO(VBO);
}

void Scene::genIBO(int index, GLuint* IBO) {
	getMesh(index).genIBO(IBO);
}

void Scene::drawMesh(int index, GLuint* VBO, GLuint* IBO, GLuint* gWVPLocation) {
	for (int index_t = 0; index_t < getMesh(index).getNumberTransformations(); index_t++) {
		WVP = getWVP() * getMesh(index).getTransformation(index_t).getFinalTransformation();
		glUniformMatrix4fv(*gWVPLocation, 1, GL_TRUE, &WVP.m[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, *VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);

		// position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		// color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(getMesh(index).getIndices().size()), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

	}
}

void Scene::genAllVBOs(GLuint* VBO) {
	for (int index_mesh = 0; index_mesh < this->meshes.size(); index_mesh++) {
		getMesh(index_mesh).genVBO(VBO);
		VBO++;  // advances pointer position
	}
}

void Scene::genAllIBOs(GLuint* IBO) {
	for (int index_mesh = 0; index_mesh < this->meshes.size(); index_mesh++) {
		getMesh(index_mesh).genIBO(IBO);
		IBO++;  // advances pointer position
	}
}

void Scene::drawAllMeshes(GLuint* VBO, GLuint* IBO, GLuint* gWVPLocation) {
	for (int index_mesh = 0; index_mesh < this->meshes.size(); index_mesh++) {
		drawMesh(index_mesh, VBO, IBO, gWVPLocation);
		VBO++;  // advances pointer position
		IBO++;  // advances pointer position
	}
}