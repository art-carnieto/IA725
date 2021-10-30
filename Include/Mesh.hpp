#pragma once
#include "Vertex.hpp"
#include "Transformation.hpp"
#include "GL/glew.h"

class Mesh {
    vector<Vertex> mesh_vertices;
    vector<unsigned int> mesh_indices;
    vector<Transformation> mesh_transformations;
    Vertex* Vertices;
    unsigned int* Indices;

public:
    Mesh();

    vector<Vertex> getVertices();
    void pushVertex(Vertex v);
    void pushVertex(float pos[3]);
    void pushVertex(float pos[3], float color[3]);
    void pushVertex(Vector3f pos, Vector3f color);
    void deleteVertex(int index);
    int getNumberVertices();

    vector<unsigned int> getIndices();
    void pushTriangleIndices(unsigned int a, unsigned int b, unsigned int c);
    void deleteTriangleIndices(int num_triangle);
    int getNumberTriangles();

    int getNumberTransformations();
    Transformation getTransformation(int index);
    void pushTransformation(Transformation t);

    void genVBO(GLuint* VBO);
    void genIBO(GLuint* IBO);

    void debug_print_vertices();
    void debug_print_indices();
};