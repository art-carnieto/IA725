#pragma once
#include "Vertex.hpp"

class Mesh {
    vector<Vertex> mesh_vertices;
    vector<unsigned int> mesh_indices;
    int first_triangle_index_position;
    int last_triangle_index_position;

public:
    
    Mesh();

    vector<Vertex> getVertices();
    void pushVertex(Vertex v);
    void pushVertex(float pos[3]);
    void pushVertex(float pos[3], float color[3]);
    void deleteVertex(int index);
    int getNumberVertices();

    vector<unsigned int> getIndices();
    void pushTriangleIndices(unsigned int a, unsigned int b, unsigned int c);
    void deleteTriangleIndices(int num_triangle);
    int getNumberTriangles();

    int getFirstTriangleIndexPos();
    void setFirstTriangleIndexPos(int pos);

    int getLastTriangleIndexPos();
    void setLastTriangleIndexPos(int pos);

    void debug_print_vertices();
    void debug_print_indices();
};