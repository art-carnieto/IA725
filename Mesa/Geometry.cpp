#include "Geometry.hpp"
#include <vector>
#include "ogldev_math_3d.h"

using std::vector;

Vertex::Vertex() {}

Vertex::Vertex(float x, float y, float z)
{
    pos = Vector3f(x, y, z);

    float red = (float)rand() / (float)RAND_MAX;
    float green = (float)rand() / (float)RAND_MAX;
    float blue = (float)rand() / (float)RAND_MAX;
    color = Vector3f(red, green, blue);
}

vector<Vertex> Vertices;
vector<unsigned int> Indices;
int indices_offset = 0;

void pushVertex(float x, float y, float z) {
    Vertices.emplace_back(Vertex(x, y, z));
}

void pushIndices(unsigned int a, unsigned int b, unsigned int c) {
    Indices.emplace_back(a + indices_offset);
    Indices.emplace_back(b + indices_offset);
    Indices.emplace_back(c + indices_offset);
}

void createCube(Vector3f position, Vector3f scale) {
    pushVertex((1 * scale[0]) + position[0], (1 * scale[1]) + position[1], (1 * scale[2]) + position[2]);
    pushVertex((1 * -scale[0]) + position[0], (1 * scale[1]) + position[1], (1 * -scale[2]) + position[2]);
    pushVertex((1 * -scale[0]) + position[0], (1 * scale[1]) + position[1], (1 * scale[2]) + position[2]);
    pushVertex((1 * scale[0]) + position[0], (1 * -scale[1]) + position[1], (1 * -scale[2]) + position[2]);
    pushVertex((1 * -scale[0]) + position[0], (1 * -scale[1]) + position[1], (1 * -scale[2]) + position[2]);
    pushVertex((1 * scale[0]) + position[0], (1 * scale[1]) + position[1], (1 * -scale[2]) + position[2]);
    pushVertex((1 * scale[0]) + position[0], (1 * -scale[1]) + position[1], (1 * scale[2]) + position[2]);
    pushVertex((1 * -scale[0]) + position[0], (1 * -scale[1]) + position[1], (1 * scale[2]) + position[2]);

    pushIndices(0, 1, 2);
    pushIndices(1, 3, 4);
    pushIndices(5, 6, 3);
    pushIndices(7, 3, 6);
    pushIndices(2, 4, 7);
    pushIndices(0, 7, 6);
    pushIndices(0, 5, 1);
    pushIndices(1, 5, 3);
    pushIndices(5, 0, 6);
    pushIndices(7, 4, 3);
    pushIndices(2, 1, 4);
    pushIndices(0, 2, 7);

    indices_offset += 8;
}