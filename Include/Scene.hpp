#pragma once
#include "Geometry.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Scene {
	vector<Mesh> meshes;
	Camera camera;
	Transformation World;
	Matrix4f WVP;

	PersProjInfo pers_info;
	OrthoProjInfo ortho_info;

	bool perspective;  //true = perspective, false = orthogonal

	Matrix4f Projection;

public:
	int mousePosX, mousePosY, lastMouseX, lastMouseY;
	bool arcball_on;

	Scene();
	Scene(Vector3f camera_pos, PersProjInfo pers_info, OrthoProjInfo ortho_info, bool perspective);

	void pushMesh(Mesh m);
	Mesh getMesh(int index);
	void clearMeshes();
	
	void setCamera(Camera cam);
	Camera getCamera();

	Transformation getWorldTransformation();
	void setWorldTransformation(Transformation t);

	Matrix4f getWVP();
	void calcWVPTransformation();

	void setNearClippingPlane(float nearZ);
	float getNearClippingPlane();

	void setFarClippingPlane(float nearZ);
	float getFarClippingPlane();

	void setPerspecive();
	void setOrthogonal();
	bool isPerspective();

	void genVBO(int index, GLuint* VBO);
	void genIBO(int index, GLuint* IBO);
	void drawMesh(int index, GLuint* VBO, GLuint* IBO, GLuint* gWVPLocation);

	void genAllVBOs(GLuint* VBO);
	void genAllIBOs(GLuint* IBO);
	void drawAllMeshes(GLuint* VBO, GLuint* IBO, GLuint* gWVPLocation);

	void moveCameraUp(float amount);
	void moveCameraDown(float amount);
	void moveCameraLeft(float amount);
	void moveCameraRight(float amount);
	void moveCameraFront(float amount);
	void moveCameraBack(float amount);

	void ArcballRotateWorld(glm::mat4 rotation);
	glm::vec3 get_arcball_vector(int x, int y);
	void computeArcball();
};