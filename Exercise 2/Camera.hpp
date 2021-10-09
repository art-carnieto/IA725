#pragma once
#include <ogldev_math_3d.h>
#include "Transformation.hpp"

class Camera {
	Transformation t_cam;
	Vector3f cameraPos;  // Camera position
	float fov;  // FOV = Field of View

public:
	Camera(Vector3f pos, float fov);

	void setCameraPos(Vector3f new_pos);
	Vector3f getCameraPos();

	void setFOV(float new_fov);
	float getFOV();

	Matrix4f getFinalTransformation();
};