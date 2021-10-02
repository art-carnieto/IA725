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

// Auxiliaries

void pushVertex(float x, float y, float z) {
    Vertices.emplace_back(Vertex(x, y, z));
}

void pushVertex(float x, float y, float z, vector<Vertex> &ver) {
    ver.emplace_back(Vertex(x, y, z));
}

void pushIndices(unsigned int a, unsigned int b, unsigned int c) {
    Indices.emplace_back(a + indices_offset);
    Indices.emplace_back(b + indices_offset);
    Indices.emplace_back(c + indices_offset);
}

void pushIndices(unsigned int a, unsigned int b, unsigned int c, vector<unsigned int> &ind) {
    ind.emplace_back(a + indices_offset);
    ind.emplace_back(b + indices_offset);
    ind.emplace_back(c + indices_offset);
}

void pushIndicesInverted(unsigned int a, unsigned int b, unsigned int c) {
    Indices.emplace_back(b + indices_offset);
    Indices.emplace_back(a + indices_offset);
    Indices.emplace_back(c + indices_offset);
}

// Object creation

void createCube(Vector3f position, Vector3f scale) {
    pushVertex((1 * (scale[0]/2)) + position[0], (1 * (scale[1]/2)) + position[1], (1 * (scale[2]/2)) + position[2]);    // +x, +y, +z
    pushVertex((1 * (-scale[0]/2)) + position[0], (1 * (scale[1]/2)) + position[1], (1 * (-scale[2]/2)) + position[2]);  // -x, +y, -z
    pushVertex((1 * (-scale[0]/2)) + position[0], (1 * (scale[1]/2)) + position[1], (1 * (scale[2]/2)) + position[2]);   // -x, +y, +z
    pushVertex((1 * (scale[0]/2)) + position[0], (1 * (-scale[1]/2)) + position[1], (1 * (-scale[2]/2)) + position[2]);  // +x, -y, -z
    pushVertex((1 * (-scale[0]/2)) + position[0], (1 * (-scale[1]/2)) + position[1], (1 * (-scale[2]/2)) + position[2]); // -x, -y, -z
    pushVertex((1 * (scale[0]/2)) + position[0], (1 * (scale[1]/2)) + position[1], (1 * (-scale[2]/2)) + position[2]);   // +x, +y, -z
    pushVertex((1 * (scale[0]/2)) + position[0], (1 * (-scale[1]/2)) + position[1], (1 * (scale[2]/2)) + position[2]);   // +x, -y, +z
    pushVertex((1 * (-scale[0]/2)) + position[0], (1 * (-scale[1]/2)) + position[1], (1 * (scale[2]/2)) + position[2]);  // -x, -y, +z

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

void createCubeUsingBoundaries(Vector3f position,
                              float x_left_boundary, float x_right_boundary, 
                              float y_up_boundary, float y_down_boundary,
                              float z_front_boundary, float z_back_boundary) {
    pushVertex(x_right_boundary + position[0], y_up_boundary + position[1], z_front_boundary + position[2]);   // +x, +y, +z
    pushVertex(x_left_boundary + position[0], y_up_boundary + position[1], z_back_boundary + position[2]);     // -x, +y, -z
    pushVertex(x_left_boundary + position[0], y_up_boundary + position[1], z_front_boundary + position[2]);    // -x, +y, +z
    pushVertex(x_right_boundary + position[0], y_down_boundary + position[1], z_back_boundary + position[2]);  // +x, -y, -z
    pushVertex(x_left_boundary + position[0], y_down_boundary + position[1], z_back_boundary + position[2]);   // -x, -y, -z
    pushVertex(x_right_boundary + position[0], y_up_boundary + position[1], z_back_boundary + position[2]);    // +x, +y, -z
    pushVertex(x_right_boundary + position[0], y_down_boundary + position[1], z_front_boundary + position[2]); // +x, -y, +z
    pushVertex(x_left_boundary + position[0], y_down_boundary + position[1], z_front_boundary + position[2]);  // -x, -y, +z

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

void createTable(Vector3f position,
                 float table_length, float table_height, float table_width,
                 float tabletop_thickness, float tableleg_length, float tableleg_width) {
    // tabletop
    createCube({position[0], position[1] + table_height - (tabletop_thickness/2), position[2]},
               {table_length, tabletop_thickness, table_width});

    // table legs
    // left back
    createCubeUsingBoundaries({position[0], position[1], position[2]}, 
        -(table_length / 2), -(table_length / 2) + tableleg_length,
        table_height - tabletop_thickness, -table_height,
        -(table_width / 2) + tableleg_width, -(table_width / 2));
    
    // left front
    createCubeUsingBoundaries({ position[0], position[1], position[2] },
        -(table_length / 2), -(table_length / 2) + tableleg_length,
        table_height - tabletop_thickness, -table_height,
        (table_width / 2), (table_width / 2) - tableleg_width);
    
    // right back
    createCubeUsingBoundaries({ position[0], position[1], position[2] },
        (table_length / 2) - tableleg_length, (table_length / 2),
        table_height - tabletop_thickness, -table_height,
        -(table_width / 2) + tableleg_width, -(table_width / 2));

    // right front
    createCubeUsingBoundaries({ position[0], position[1], position[2] },
        (table_length / 2) - tableleg_length, (table_length / 2),
        table_height - tabletop_thickness, -table_height,
        (table_width / 2), (table_width / 2) - tableleg_width);
}

void createRegularIcosahedron(Vector3f position, Vector3f scale) {
    // those coordinates are pre-calculated
    float x = 0.525731112119133606f;
    float z = 0.850650808352039932f;
    float y = 0.0f;

    pushVertex(position[0] - (x * scale[0]), position[1] + (y * scale[1]), position[2] + (z * scale[2]));
    pushVertex(position[0] + (x * scale[0]), position[1] + (y * scale[1]), position[2] + (z * scale[2]));
    pushVertex(position[0] - (x * scale[0]), position[1] + (y * scale[1]), position[2] - (z * scale[2]));
    pushVertex(position[0] + (x * scale[0]), position[1] + (y * scale[1]), position[2] - (z * scale[2]));

    pushVertex(position[0] + (y * scale[0]), position[1] + (z * scale[1]), position[2] + (x * scale[2]));
    pushVertex(position[0] + (y * scale[0]), position[1] + (z * scale[1]), position[2] - (x * scale[2]));
    pushVertex(position[0] + (y * scale[0]), position[1] - (z * scale[1]), position[2] + (x * scale[2]));
    pushVertex(position[0] + (y * scale[0]), position[1] - (z * scale[1]), position[2] - (x * scale[2]));

    pushVertex(position[0] + (z * scale[0]), position[1] + (x * scale[1]), position[2] + (y * scale[2]));
    pushVertex(position[0] - (z * scale[0]), position[1] + (x * scale[1]), position[2] + (y * scale[2]));
    pushVertex(position[0] + (z * scale[0]), position[1] - (x * scale[1]), position[2] + (y * scale[2]));
    pushVertex(position[0] - (z * scale[0]), position[1] - (x * scale[1]), position[2] + (y * scale[2]));

    pushIndicesInverted(0, 4, 1);
    pushIndicesInverted(0, 9, 4);
    pushIndicesInverted(9, 5, 4);
    pushIndicesInverted(4, 5, 8);
    pushIndicesInverted(4, 8, 1);

    pushIndicesInverted(8, 10, 1);
    pushIndicesInverted(8, 3, 10);
    pushIndicesInverted(5, 3, 8);
    pushIndicesInverted(5, 2, 3);
    pushIndicesInverted(2, 7, 3);

    pushIndicesInverted(7, 10, 3);
    pushIndicesInverted(7, 6, 10);
    pushIndicesInverted(7, 11, 6);
    pushIndicesInverted(11, 0, 6);
    pushIndicesInverted(0, 1, 6);

    pushIndicesInverted(6, 1, 10);
    pushIndicesInverted(9, 0, 11);
    pushIndicesInverted(9, 11, 2);
    pushIndicesInverted(9, 2, 5);
    pushIndicesInverted(7, 2, 11);

    indices_offset += 12;
}