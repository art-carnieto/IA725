#pragma once
#include "ogldev_math_3d.h"

class Vertex {
    Vector3f pos;
    Vector3f color;
    Vector3f normal;

public:
    Vertex();
    Vertex(float x, float y, float z);
    Vertex(float x, float y, float z, float r, float g, float b);
    Vertex(float x, float y, float z, float r, float g, float b, float nx, float ny, float nz);
    Vertex(Vector3f pos);
    Vertex(Vector3f pos, Vector3f color);
    Vertex(Vector3f pos, Vector3f color, Vector3f normal);
    Vector3f getPosition();
    Vector3f getColor();
    Vector3f getNormal();
    void setColor(float r, float g, float b);
    void setColor(Vector3f new_color);
    void setNormal(float nx, float ny, float nz);
    void setNormal(Vector3f new_normal);
};