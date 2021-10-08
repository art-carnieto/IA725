// intensive use of math_3d.cpp and ogldev_math_3d.h
#include "Transformation.hpp"

Transformation::Transformation() {
	this->translation.InitIdentity();
	this->scale.InitIdentity();
	this->rotation.InitIdentity();
	this->final_transformation.InitIdentity();
};

void Transformation::setScale(Vector3f scale) {
	this->scale.InitScaleTransform(scale[0], scale[1], scale[2]);
}

Matrix4f Transformation::getScale() {
	return this->scale;
}

void Transformation::setRotation(Vector3f rotation) {
	this->rotation.InitRotateTransform(rotation[0], rotation[1], rotation[2]);
}

Matrix4f Transformation::getRotation() {
	return this->rotation;
}

void Transformation::setTranslation(Vector3f translation) {
	this->translation.InitTranslationTransform(translation[0], translation[1], translation[2]);
}

Matrix4f Transformation::getTranslation() {
	return this->translation;
}

void Transformation::calcFinalTransformation() {
	this->final_transformation = this->translation * this->rotation * this->scale;
}

Matrix4f Transformation::getFinalTransformation() {
	return this->final_transformation;
}