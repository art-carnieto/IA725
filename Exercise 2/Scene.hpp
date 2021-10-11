#pragma once
#include "Geometry.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include <GL/glew.h>

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

	void setClippingPlanes(float nearZ, float farZ);
	float* getClippingPlanes();

	void setPerspecive();
	void setOrthogonal();
	bool isPerspective();

	void genVBO(int index, GLuint* VBO);
	void genIBO(int index, GLuint* IBO);
	void drawMesh(int index, GLuint* VBO, GLuint* IBO, GLuint* gWVPLocation);

	void genAllVBOs(GLuint* VBO);
	void genAllIBOs(GLuint* IBO);
	void drawAllMeshes(GLuint* VBO, GLuint* IBO, GLuint* gWVPLocation);
};