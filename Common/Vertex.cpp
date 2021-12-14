#include "Vertex.hpp"

Vertex::Vertex() {}

Vertex::Vertex(float x, float y, float z)   // create vertex using just positions, colors are random
{
    pos = Vector3f(x, y, z);

    float red = (float)rand() / (float)RAND_MAX;
    float green = (float)rand() / (float)RAND_MAX;
    float blue = (float)rand() / (float)RAND_MAX;
    color = Vector3f(red, green, blue);
    normal = Vector3f({ 0.0f, 0.0f, 0.0f });
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    pos = Vector3f(x, y, z);
    color = Vector3f(r, g, b);
    normal = Vector3f({ 0.0f, 0.0f, 0.0f });
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float nx, float ny, float nz)
{
    pos = Vector3f(x, y, z);
    color = Vector3f(r, g, b);
    normal = Vector3f(nx, ny, nz);
}

Vertex::Vertex(Vector3f pos) {  // create vertex using just positions, colors are random
    this->pos = pos;

    float red = (float)rand() / (float)RAND_MAX;
    float green = (float)rand() / (float)RAND_MAX;
    float blue = (float)rand() / (float)RAND_MAX;
    this->color = Vector3f(red, green, blue);
    this->normal = Vector3f({ 0.0f, 0.0f, 0.0f });
}

Vertex::Vertex(Vector3f pos, Vector3f color) {
    this->pos = pos;
    this->color = color;
    this->normal = Vector3f({ 0.0f, 0.0f, 0.0f });
}

Vertex::Vertex(Vector3f pos, Vector3f color, Vector3f normal) {
    this->pos = pos;
    this->color = color;
    this->normal = normal;
}

Vector3f Vertex::getPosition() {
    return this->pos;
}

Vector3f Vertex::getColor() {
    return this->color;
}

Vector3f Vertex::getNormal() {
    return this->normal;
}

void Vertex::setColor(float r, float g, float b) {
    this->color = Vector3f(r, g, b);
}

void Vertex::setColor(Vector3f new_color) {
    this->color = new_color;
}

void Vertex::setNormal(float nx, float ny, float nz) {
    this->normal = Vector3f(nx, ny, nz);
}

void Vertex::setNormal(Vector3f new_normal) {
    this->normal = Vector3f(new_normal);
}