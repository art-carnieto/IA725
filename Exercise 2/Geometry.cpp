#include "Geometry.hpp"

// Auxiliaries
void normalizeVertexPositions(float v[3]) {
    float d = sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
    if (d == 0.0) return;
    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

void subdivideTriangle(int num_triangle, Mesh &m) {
    int index = num_triangle * 3;
    assert(index < m.getIndices().size()); // avoid index overflow on ind

    unsigned int ind_a = m.getIndices()[index];
    unsigned int ind_b = m.getIndices()[index + 1];
    unsigned int ind_c = m.getIndices()[index + 2];

    Vertex ver_1 = m.getVertices()[ind_a];
    Vertex ver_2 = m.getVertices()[ind_b];
    Vertex ver_3 = m.getVertices()[ind_c];

    float new_vertex_12[3] = { 
        ver_1.getPosition()[0] + ver_2.getPosition()[0],
        ver_1.getPosition()[1] + ver_2.getPosition()[1],
        ver_1.getPosition()[2] + ver_2.getPosition()[2]
    };

    float new_vertex_23[3] = {
        ver_2.getPosition()[0] + ver_3.getPosition()[0],
        ver_2.getPosition()[1] + ver_3.getPosition()[1],
        ver_2.getPosition()[2] + ver_3.getPosition()[2]
    };

    float new_vertex_31[3] = {
        ver_3.getPosition()[0] + ver_1.getPosition()[0],
        ver_3.getPosition()[1] + ver_1.getPosition()[1],
        ver_3.getPosition()[2] + ver_1.getPosition()[2]
    };

    normalizeVertexPositions(new_vertex_12);
    normalizeVertexPositions(new_vertex_23);
    normalizeVertexPositions(new_vertex_31);

    unsigned int new_index_12 = (unsigned int) m.getNumberVertices();
    unsigned int new_index_23 = (unsigned int) m.getNumberVertices() + 1;
    unsigned int new_index_31 = (unsigned int) m.getNumberVertices() + 2;

    m.pushVertex(new_vertex_12, ver_1.getColor());
    m.pushVertex(new_vertex_23, ver_2.getColor());
    m.pushVertex(new_vertex_31, ver_3.getColor());

    m.pushTriangleIndices(ind_a, new_index_12, new_index_31);
    m.pushTriangleIndices(ind_b, new_index_23, new_index_12);
    m.pushTriangleIndices(ind_c, new_index_31, new_index_23);
    m.pushTriangleIndices(new_index_12, new_index_23, new_index_31);

    m.deleteTriangleIndices(num_triangle);
}

// Object creation

Mesh createCube(Vector3f color) {
    Mesh m;
    m.pushVertex({ 0.5f, 0.5f, 0.5f }, color);
    m.pushVertex({ -0.5f, 0.5f, -0.5f }, color);
    m.pushVertex({ -0.5f, 0.5f, 0.5f }, color);
    m.pushVertex({ 0.5f, -0.5f, -0.5f }, color);
    m.pushVertex({ -0.5f, -0.5f, -0.5f }, color);
    m.pushVertex({ 0.5f, 0.5f, -0.5f }, color);
    m.pushVertex({ 0.5f, -0.5f, 0.5f }, color);
    m.pushVertex({ -0.5f, -0.5f, 0.5f }, color);

    m.pushTriangleIndices(0, 1, 2);
    m.pushTriangleIndices(1, 3, 4);
    m.pushTriangleIndices(5, 6, 3);
    m.pushTriangleIndices(7, 3, 6);
    m.pushTriangleIndices(2, 4, 7);
    m.pushTriangleIndices(0, 7, 6);
    m.pushTriangleIndices(0, 5, 1);
    m.pushTriangleIndices(1, 5, 3);
    m.pushTriangleIndices(5, 0, 6);
    m.pushTriangleIndices(7, 4, 3);
    m.pushTriangleIndices(2, 1, 4);
    m.pushTriangleIndices(0, 2, 7);

    return m;
}

Mesh createRegularIcosahedron(Vector3f color) {
    Mesh m;
    // those coordinates are pre-calculated
    const float x = 0.525731112119133606f;
    const float z = 0.850650808352039932f;
    const float y = 0.0f;

    m.pushVertex({ -x, y, z }, color);
    m.pushVertex({ x, y, z }, color);
    m.pushVertex({ -x, y, -z }, color);
    m.pushVertex({ x, y, -z }, color);
    m.pushVertex({ y, z, x }, color);
    m.pushVertex({ y, z, -x }, color);
    m.pushVertex({ y, -z, x }, color);
    m.pushVertex({ y, -z, -x }, color);
    m.pushVertex({ z, x, y }, color);
    m.pushVertex({ -z, x, y }, color);
    m.pushVertex({ z, -x, y }, color);
    m.pushVertex({ -z, -x, y }, color);

    m.pushTriangleIndices(4, 0, 1);
    m.pushTriangleIndices(9, 0, 4);
    m.pushTriangleIndices(5, 9, 4);
    m.pushTriangleIndices(5, 4, 8);
    m.pushTriangleIndices(8, 4, 1);

    m.pushTriangleIndices(10, 8, 1);
    m.pushTriangleIndices(3, 8, 10);
    m.pushTriangleIndices(3, 5, 8);
    m.pushTriangleIndices(2, 5, 3);
    m.pushTriangleIndices(7, 2, 3);

    m.pushTriangleIndices(10, 7, 3);
    m.pushTriangleIndices(6, 7, 10);
    m.pushTriangleIndices(11, 7, 6);
    m.pushTriangleIndices(0, 11, 6);
    m.pushTriangleIndices(1, 0, 6);

    m.pushTriangleIndices(1, 6, 10);
    m.pushTriangleIndices(0, 9, 11);
    m.pushTriangleIndices(11, 9, 2);
    m.pushTriangleIndices(2, 9, 5);
    m.pushTriangleIndices(2, 7, 11);

    return m;
}

Mesh createSubdividedIcosahedron(int subdiv, Vector3f color) {
    Mesh icosahedron = createRegularIcosahedron(color);

    auto indices_size = icosahedron.getIndices().size();
    for (int i = 0; i < subdiv; i++) {
        indices_size = icosahedron.getIndices().size();
        for (int triangle = 0; triangle < indices_size; triangle+=3) {
            subdivideTriangle(0, icosahedron);
        }
    }

    return icosahedron;
}