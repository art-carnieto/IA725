#pragma once
#include "Geometry.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"

class Scene {
	vector<Mesh> meshes;
	Camera camera;
	Transformation World;
	Transformation WVP;

	float ar;     // aspect ratio
	float nearZ;  // near plane clipping
	float farZ;   // far plane clipping

	bool perspective;  //true = perspective, false = orthogonal

public:
	Scene();
	Scene(Vector3f camera_pos, float fov, float ar, float nearZ, float farZ, bool perspective);

	void pushMesh(Mesh m);
	Mesh getMesh(int index);
	void clearMeshes();
	
	void setCamera(Camera cam);
	Camera getCamera();

	Transformation getWorldTransformation();
	void setWorldTransformation(Transformation t);

	Transformation getWVPTransformation();
	void setWVPTransformation(Transformation t);

	void setAR(float ar);
	void setAR(float width, float height);
	float getAR();

	void setClippingPlanes(float nearZ, float farZ);
	float* getClippingPlanes();

	void setPerspecive();
	void setOrthogonal();
	bool isPerspective();
};