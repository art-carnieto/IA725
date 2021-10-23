#pragma once
#include "ogldev_math_3d.h"

class Vertex {
    Vector3f pos;
    Vector3f color;

public:
    Vertex();
    Vertex(float x, float y, float z);
    Vertex(float x, float y, float z, float r, float g, float b);
    Vector3f getPosition();
    Vector3f getColor();
};