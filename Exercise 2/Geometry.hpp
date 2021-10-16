#pragma once
#include "Mesh.hpp"
#include "Transformation.hpp"

// to fix 'M_PI' not declared error
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Geometry
void subdivideTriangle(int num_triangle, Mesh &m);
void normalizeVertexPositions(float v[3]);
bool cmp_eq_float(float x, float y, float epsilon);

// Object creation
Mesh createCube(Vector3f color);
Mesh createRegularIcosahedron(Vector3f color);
Mesh createSubdividedIcosahedron(int subdiv, Vector3f color);
Mesh createPlane(float delta, Vector3f color);
Mesh createCircle(int subdiv, float radius, Vector3f color);
Mesh createCone(int subdiv, float radius, float height, Vector3f color);
Mesh createCylinder(int subdiv, float radius, float height, Vector3f color);
Mesh createSphere(int xSegments, int ySegments, float radius, Vector3f color);