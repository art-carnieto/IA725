#pragma once
#include "Vertex.hpp"

class Mesh {
    vector<Vertex> mesh_vertices;
    vector<unsigned int> mesh_indices;

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

    void debug_print_vertices();
    void debug_print_indices();
};