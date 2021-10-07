#pragma once
#include "Mesh.hpp"

// to fix 'M_PI' not declared error
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Geometry
void subdivideTriangle(int num_triangle, Mesh &m);
void normalizeVertexPositions(float v[3]);

// Object creation
Mesh createCube(Vector3f color);
void createTable(Vector3f position,
    float table_length, float table_height, float table_width,
    float tabletop_thickness, float tableleg_length, float tableleg_width,
    Vector3f color);
Mesh createRegularIcosahedron(Vector3f color);
Mesh createSubdividedIcosahedron(int subdiv, Vector3f color);