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

// Auxiliaries
void pushVertex(float x, float y, float z);
void pushVertex(float x, float y, float z, vector<Vertex>& ver);
void pushIndices(unsigned int a, unsigned int b, unsigned int c);
void pushIndices(unsigned int a, unsigned int b, unsigned int c, vector<unsigned int> &ind);
void pushIndicesInverted(unsigned int a, unsigned int b, unsigned int c);
//void pushIndicesWithoutOffset(unsigned int a, unsigned int b, unsigned int c);
//int findTriangleUsingIndices(unsigned int a, unsigned int b, unsigned int c);
void deleteTriangle(int num_triangle, vector<unsigned int>& ind);
void deleteTriangle(int num_triangle);
void deleteVertex(int index, vector<Vertex>& ver);
void deleteVertex(int index);
//void subdivideTriangle(int num_triangle,
//    vector<Vertex> old_obj_vertices, vector<unsigned int> old_obj_indices,
//    vector<Vertex> new_obj_vertices, vector<unsigned int> new_obj_indices);

// Object creation
void createCube(Vector3f position, Vector3f scale);
void createCubeUsingBoundaries(Vector3f position, float x_left_boundary, float x_right_boundary, float y_up_boundary, float y_down_boundary, float z_front_boundary, float z_back_boundary);
void createTable(Vector3f position, float table_length, float table_height, float table_width, float tabletop_thickness, float tableleg_length, float tableleg_width);
void createRegularIcosahedron(Vector3f position, Vector3f scale);
//void createSubdividedIcosahedron(int subdiv, Vector3f position, Vector3f scale);