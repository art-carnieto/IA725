// intensive use of math_3d.cpp and ogldev_math_3d.h
#pragma once
#include "ogldev_math_3d.h"
#include "glm/glm.hpp"


class Transformation {
	Matrix4f final_transformation;
	Matrix4f translation;
	Matrix4f rotation;
	Matrix4f scale;

public:
	Transformation();
	void setScale(Vector3f scale);
	Matrix4f getScale();

	void setRotation(Vector3f rotation);
	Matrix4f getRotation();
	void setRotation(glm::mat4 rotation);
	
	void setTranslation(Vector3f translation);
	Matrix4f getTranslation();

	void calcFinalTransformation();
	Matrix4f getFinalTransformation();
};