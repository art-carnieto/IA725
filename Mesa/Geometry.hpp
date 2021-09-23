#pragma once
#include <vector>
#include "ogldev_math_3d.h"

// to fix 'M_PI' not declared error
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using std::vector;

struct Vertex {
    Vector3f pos;
    Vector3f color;

    Vertex();
    Vertex(float x, float y, float z);
};

extern vector<Vertex> Vertices;
extern vector<unsigned int> Indices;
extern int indices_offset;

void pushVertex(float x, float y, float z);
void pushIndices(unsigned int a, unsigned int b, unsigned int c);
void createCube(Vector3f position, Vector3f scale);