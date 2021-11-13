#include "Vertex.hpp"

Vertex::Vertex() {}

Vertex::Vertex(float x, float y, float z)   // create vertex using just positions, colors are random
{
    pos = Vector3f(x, y, z);

    float red = (float)rand() / (float)RAND_MAX;
    float green = (float)rand() / (float)RAND_MAX;
    float blue = (float)rand() / (float)RAND_MAX;
    color = Vector3f(red, green, blue);
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    pos = Vector3f(x, y, z);
    color = Vector3f(r, g, b);
}

Vector3f Vertex::getPosition() {
    return Vertex::pos;
}

Vector3f Vertex::getColor() {
    return Vertex::color;
}

void Vertex::setColor(float r, float g, float b) {
    this->color = Vector3f(r, g, b);
}

void Vertex::setColor(Vector3f new_color) {
    this->color = new_color;
}