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
Vertex BernsteinCurve(float u, Vertex points[4], Vector3f color);
Vertex BernsteinSurface(float u, float v, Vertex patch_points[4][4], Vector3f color);
vector<Vertex> genPatchBezier(Vertex patch_points[4][4], int divs, Vector3f color);
vector<Vertex> genPatchBezierUsingIndices(vector<Vertex> list_control_points, unsigned int indices[16], int divs, Vector3f color);

// Object creation
Mesh createCube(Vector3f color);
Mesh createTable(Vector3f position,
    float table_length, float table_height, float table_width,
    float tabletop_thickness, float tableleg_length, float tableleg_width,
    Vector3f color);
Mesh createRegularIcosahedron(Vector3f color);
Mesh createSubdividedIcosahedron(int subdiv, Vector3f color);
Mesh createPlane(float delta, Vector3f color);
Mesh createCircle(int subdiv, float radius, Vector3f color);
Mesh createCone(int subdiv, float radius, float height, Vector3f color);
Mesh createCylinder(int subdiv, float radius, float height, Vector3f color);
Mesh createSphere(int xSegments, int ySegments, float radius, Vector3f color);
Mesh createCubicBezierMesh(vector<Vertex> list_control_points, vector<unsigned int> bezier_indices, int subdiv, Vector3f color);
Mesh createUtahTeapot(int subdiv, Vector3f color);