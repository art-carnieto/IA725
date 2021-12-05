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
	this->mousePosX, this->mousePosY = 0;
	this->lastMouseX, this->lastMouseY = 0;
	this->arcball_on = false;
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

void Scene::setNearClippingPlane(float nearZ) {
	this->pers_info.zNear = nearZ;
	this->ortho_info.n = nearZ;
}

float Scene::getNearClippingPlane() {
	return this->pers_info.zNear;
}

void Scene::setFarClippingPlane(float farZ) {
	this->pers_info.zFar = farZ;
	this->ortho_info.f = farZ;
}

float Scene::getFarClippingPlane() {
	return this->pers_info.zFar;
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
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	if (getMesh(index).getUsesIndices()) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

	// loop through all transformations of the Mesh and draw each one of them
	for (int index_t = 0; index_t < getMesh(index).getNumberTransformations(); index_t++) {
		WVP = getWVP() * getMesh(index).getTransformation(index_t).getFinalTransformation();
		glUniformMatrix4fv(*gWVPLocation, 1, GL_TRUE, &WVP.m[0][0]);

		if (getMesh(index).getUsesIndices())
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(getMesh(index).getIndices().size()), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, getMesh(index).getNumberVertices());
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
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

void Scene::moveCameraUp(float amount) {
	this->camera.moveUp(amount);
}

void Scene::moveCameraDown(float amount) {
	this->camera.moveDown(amount);
}

void Scene::moveCameraLeft(float amount) {
	this->camera.moveLeft(amount);
}

void Scene::moveCameraRight(float amount) {
	this->camera.moveRight(amount);
}

void Scene::moveCameraFront(float amount) {
	this->camera.moveFront(amount);
}

void Scene::moveCameraBack(float amount) {
	this->camera.moveBack(amount);
}

void Scene::ArcballRotateWorld(glm::mat4 rotation) {
	this->World.setRotation(rotation);
}

/**
 * Adapted from https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
 * Get a normalized vector from the center of the virtual ball O to a
 * point P on the virtual ball surface, such that P is aligned on
 * screen's (X,Y) coordinates.  If (X,Y) is too far away from the
 * sphere, return the nearest point on the virtual ball surface.
 */
glm::vec3 Scene::get_arcball_vector(int x, int y) {
	glm::vec3 P = glm::vec3(1.0 * x / this->pers_info.Width * 2 - 1.0,
		1.0 * y / this->pers_info.Height * 2 - 1.0,
		0);
	P.y = -P.y;
	float OP_squared = P.x * P.x + P.y * P.y;
	if (OP_squared <= 1 * 1)
		P.z = sqrt(1 * 1 - OP_squared);  // Pythagoras
	else
		P = glm::normalize(P);  // nearest point
	return P;
}

// Adapted from https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
void Scene::computeArcball() {
	if (this->mousePosX != this->lastMouseX || this->mousePosY != this->lastMouseY) {
		glm::vec3 va = get_arcball_vector(lastMouseX, lastMouseY);
		glm::vec3 vb = get_arcball_vector(mousePosX, mousePosY);
		float angle = acos(min(1.0f, glm::dot(va, vb)));
		glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
		
		float floatWVP[9];
		Matrix4f currentWVP = getWVP();
		int cont = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++) {
				floatWVP[cont] = currentWVP.m[i][j];
				cont++;
			}

		glm::mat3 camera2object = glm::inverse(glm::make_mat3(floatWVP));
		glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;

		float worldRotation[16];
		cont = 0;
		for (int i = 0; i < 4; i++) 
			for (int j = 0; j < 4; j++) {
				worldRotation[cont] = this->World.getRotation().m[i][j];
				cont++;
			}

		float intensity = 0.05f;  // this number was chosen by testing
		glm::mat4 object2world = glm::make_mat4(worldRotation);
		object2world = glm::rotate(object2world, glm::degrees(angle * intensity), axis_in_object_coord);
		ArcballRotateWorld(object2world);

		lastMouseX = mousePosX;
		lastMouseY = mousePosY;
	}
}