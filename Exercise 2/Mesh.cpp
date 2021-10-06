#include "Mesh.hpp"
#include <iostream>

Mesh::Mesh() {
    // fist and last positions start as -1 (invalid)
    // that means this mesh is not in a scene yet
	setFirstTriangleIndexPos(-1);
	setLastTriangleIndexPos(-1);
}

// Vertices
vector<Vertex> Mesh::getVertices() {
    return this->mesh_vertices;
}

void Mesh::pushVertex(Vertex v) {
    this->mesh_vertices.emplace_back(v);
}

void Mesh::pushVertex(float pos[3]) {
    this->mesh_vertices.emplace_back(Vertex(pos[0], pos[1], pos[2]));
}

void Mesh::pushVertex(float pos[3], float color[3]) {
    this->mesh_vertices.emplace_back(Vertex(pos[0], pos[1], pos[2], color[0], color[1], color[2]));
}

void Mesh::deleteVertex(int index) {
    assert(index > Mesh::getNumberVertices() - 1); // avoid index overflow on Vertices
    this->mesh_vertices.erase(this->mesh_vertices.begin() + index);
    // FIXME: it needs to erase all the triangles related to that vertex 
    // in the indices list and correct indices list!
}

int Mesh::getNumberVertices() {
    return static_cast<int>(this->mesh_vertices.size());
}

// Indices
vector<unsigned int> Mesh::getIndices() {
    return this->mesh_indices;
}

void Mesh::pushTriangleIndices(unsigned int a, unsigned int b, unsigned int c) {
    this->mesh_indices.emplace_back(a);
    this->mesh_indices.emplace_back(b);
    this->mesh_indices.emplace_back(c);
}

void Mesh::deleteTriangleIndices(int num_triangle) {
    int index = num_triangle * 3;
    assert(index > this->mesh_indices.size() - 1); // avoid index overflow on Indices 
    this->mesh_indices.erase(this->mesh_indices.begin() + index, this->mesh_indices.begin() + index + 3);
}

int Mesh::getNumberTriangles() {
    return static_cast<int>(this->mesh_indices.size() / 3);
}

// Indices positions in scene
int Mesh::getFirstTriangleIndexPos() {
    return first_triangle_index_position;
}

void Mesh::setFirstTriangleIndexPos(int pos) {
    first_triangle_index_position = pos;
}

int Mesh::getLastTriangleIndexPos() {
    return last_triangle_index_position;
}

void Mesh::setLastTriangleIndexPos(int pos) {
    last_triangle_index_position = pos;
}

// Debug
void Mesh::debug_print_vertices() {
    cout << "\n*** DEBUG VERTICES ***\n";
    cout << "Number of vertices = " << Mesh::getNumberVertices() << endl;
    cout << "sizeof() first vertex = " << sizeof(this->mesh_vertices[0]) << endl;
    cout << "Number of vertices * sizeof() first vertex = " << Mesh::getNumberVertices() * sizeof(this->mesh_vertices[0]) << endl;
    for (int i = 0; i < Mesh::getNumberVertices(); i++) {
        cout << "pos: { " << this->mesh_vertices[i].getPosition()[0] << " " << this->mesh_vertices[i].getPosition()[1] << " " << this->mesh_vertices[i].getPosition()[2] << " }\n";
        cout << "color: { " << this->mesh_vertices[i].getColor()[0] << " " << this->mesh_vertices[i].getColor()[1] << " " << this->mesh_vertices[i].getColor()[2] << " }\n";
    }
}

void Mesh::debug_print_indices() {
    cout << "\n*** DEBUG INDICES ***\n";
    cout << "Number of indices = " << this->mesh_indices.size() << endl;
    cout << "sizeof() first index = " << sizeof(this->mesh_indices[0]) << endl;
    cout << "Number of indices * sizeof() first index = " << this->mesh_indices.size() * sizeof(this->mesh_indices[0]) << endl;

    int j = 0;
    for (int i = 0; i < this->mesh_indices.size(); i++) {
        cout << this->mesh_indices[i] << "  ";
        j++;
        if (j > 2) {
            cout << endl;
            j = 0;
        }
    }
}