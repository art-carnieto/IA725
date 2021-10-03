#include "Geometry.hpp"
#include <vector>
#include "ogldev_math_3d.h"
#include <iostream> // just to use cout to print on terminal

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

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    pos = Vector3f(x, y, z);
    color = Vector3f(r, g, b);
}

vector<Vertex> Vertices;
vector<unsigned int> Indices;
int indices_offset = 0;

// Auxiliaries

void pushVertex(float x, float y, float z, vector<Vertex> &ver) {
    ver.emplace_back(Vertex(x, y, z));
}

void pushVertex(float x, float y, float z) {
    pushVertex(x, y, z, Vertices);
}

void pushVertex(float x, float y, float z, float r, float g, float b, vector<Vertex>& ver) {
    ver.emplace_back(Vertex(x, y, z, r, g, b));
}

void pushVertex(float x, float y, float z, float r, float g, float b) {
    pushVertex(x, y, z, r, g, b, Vertices);
}

void pushIndices(unsigned int a, unsigned int b, unsigned int c, vector<unsigned int> &ind) {
    ind.emplace_back(a + indices_offset);
    ind.emplace_back(b + indices_offset);
    ind.emplace_back(c + indices_offset);
}

void pushIndices(unsigned int a, unsigned int b, unsigned int c) {
    pushIndices(a, b, c, Indices);
}

void pushIndicesInverted(unsigned int a, unsigned int b, unsigned int c) {
    pushIndices(b, a, c, Indices);
}

void pushIndicesWithoutOffset(unsigned int a, unsigned int b, unsigned int c, vector<unsigned int> &ind) {
    Indices.emplace_back(a);
    Indices.emplace_back(b);
    Indices.emplace_back(c);
}

void pushIndicesWithoutOffset(unsigned int a, unsigned int b, unsigned int c) {
    pushIndicesWithoutOffset(a, b, c, Indices);
}

void deleteTriangle(int num_triangle, vector<unsigned int> &ind) {
    int index = num_triangle * 3;
    if (index > ind.size() - 1) return; // avoid index overflow on Indices 
    ind.erase(ind.begin() + index, ind.begin() + index + 3);
}

void deleteTriangle(int num_triangle) {
    deleteTriangle(num_triangle, Indices);
}

void deleteVertex(int index, vector<Vertex> &ver) {
    if (index > ver.size() - 1) return; // avoid index overflow on Vertices
    ver.erase(ver.begin() + index);
    // FIXME: it needs to erase all the triangles related to that vertex and fix indices list!
}

void deleteVertex(int index) {
    deleteVertex(index, Vertices);
}

void normalizeVertexPositions(float v[3]) {
    float d = sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
    if (d == 0.0) return;
    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

void subdivideTriangle(int num_triangle, vector<Vertex> &ver, vector<unsigned int> &ind) {
    int index = num_triangle * 3;
    if (index > ind.size() - 1) return; // avoid index overflow on ind

    unsigned int ind_a = ind.at(index);
    unsigned int ind_b = ind.at(index + 1);
    unsigned int ind_c = ind.at(index + 2);

    Vertex ver_1 = ver.at(ind_a);
    Vertex ver_2 = ver.at(ind_b);
    Vertex ver_3 = ver.at(ind_c);

    float new_vertex_12[3] = { 
        ver_1.pos[0] + ver_2.pos[0],
        ver_1.pos[1] + ver_2.pos[1],
        ver_1.pos[2] + ver_2.pos[2]
    };

    float new_vertex_23[3] = {
        ver_2.pos[0] + ver_3.pos[0],
        ver_2.pos[1] + ver_3.pos[1],
        ver_2.pos[2] + ver_3.pos[2]
    };

    float new_vertex_31[3] = {
        ver_3.pos[0] + ver_1.pos[0],
        ver_3.pos[1] + ver_1.pos[1],
        ver_3.pos[2] + ver_1.pos[2]
    };

    normalizeVertexPositions(new_vertex_12);
    normalizeVertexPositions(new_vertex_23);
    normalizeVertexPositions(new_vertex_31);

    unsigned int new_index_12 = (unsigned int) ver.size();
    unsigned int new_index_23 = (unsigned int) ver.size() + 1;
    unsigned int new_index_31 = (unsigned int) ver.size() + 2;

    pushVertex(new_vertex_12[0], new_vertex_12[1], new_vertex_12[2], ver_1.color[0], ver_1.color[1], ver_1.color[2], ver);
    pushVertex(new_vertex_23[0], new_vertex_23[1], new_vertex_23[2], ver_2.color[0], ver_2.color[1], ver_2.color[2], ver);
    pushVertex(new_vertex_31[0], new_vertex_31[1], new_vertex_31[2], ver_3.color[0], ver_3.color[1], ver_3.color[2], ver);

    pushIndicesWithoutOffset(ind_a, new_index_12, new_index_31, ind);
    pushIndicesWithoutOffset(ind_b, new_index_23, new_index_12, ind);
    pushIndicesWithoutOffset(ind_c, new_index_31, new_index_23, ind);
    pushIndicesWithoutOffset(new_index_12, new_index_23, new_index_31, ind);

    deleteTriangle(num_triangle, ind);

    indices_offset += 3;
}

void subdivideTriangle(int num_triangle) {
    subdivideTriangle(num_triangle, Vertices, Indices);
}

// Object creation

void createCube(Vector3f position, Vector3f scale, Vector3f color) {
    if (color[0] == -1.0f && color[1] == -1.0f && color[2] == -1.0f) {    // color not defined, random colors are used instead
        pushVertex((1 * (scale[0] / 2)) + position[0], (1 * (scale[1] / 2)) + position[1], (1 * (scale[2] / 2)) + position[2]);    // +x, +y, +z
        pushVertex((1 * (-scale[0] / 2)) + position[0], (1 * (scale[1] / 2)) + position[1], (1 * (-scale[2] / 2)) + position[2]);  // -x, +y, -z
        pushVertex((1 * (-scale[0] / 2)) + position[0], (1 * (scale[1] / 2)) + position[1], (1 * (scale[2] / 2)) + position[2]);   // -x, +y, +z
        pushVertex((1 * (scale[0] / 2)) + position[0], (1 * (-scale[1] / 2)) + position[1], (1 * (-scale[2] / 2)) + position[2]);  // +x, -y, -z
        pushVertex((1 * (-scale[0] / 2)) + position[0], (1 * (-scale[1] / 2)) + position[1], (1 * (-scale[2] / 2)) + position[2]); // -x, -y, -z
        pushVertex((1 * (scale[0] / 2)) + position[0], (1 * (scale[1] / 2)) + position[1], (1 * (-scale[2] / 2)) + position[2]);   // +x, +y, -z
        pushVertex((1 * (scale[0] / 2)) + position[0], (1 * (-scale[1] / 2)) + position[1], (1 * (scale[2] / 2)) + position[2]);   // +x, -y, +z
        pushVertex((1 * (-scale[0] / 2)) + position[0], (1 * (-scale[1] / 2)) + position[1], (1 * (scale[2] / 2)) + position[2]);  // -x, -y, +z
    }
    else {
        pushVertex((1 * (scale[0]/2)) + position[0], (1 * (scale[1]/2)) + position[1], (1 * (scale[2]/2)) + position[2], color[0], color[1], color[2]);    // +x, +y, +z
        pushVertex((1 * (-scale[0] / 2)) + position[0], (1 * (scale[1] / 2)) + position[1], (1 * (-scale[2] / 2)) + position[2], color[0], color[1], color[2]);  // -x, +y, -z
        pushVertex((1 * (-scale[0] / 2)) + position[0], (1 * (scale[1] / 2)) + position[1], (1 * (scale[2] / 2)) + position[2], color[0], color[1], color[2]);   // -x, +y, +z
        pushVertex((1 * (scale[0] / 2)) + position[0], (1 * (-scale[1] / 2)) + position[1], (1 * (-scale[2] / 2)) + position[2], color[0], color[1], color[2]);  // +x, -y, -z
        pushVertex((1 * (-scale[0] / 2)) + position[0], (1 * (-scale[1] / 2)) + position[1], (1 * (-scale[2] / 2)) + position[2], color[0], color[1], color[2]); // -x, -y, -z
        pushVertex((1 * (scale[0] / 2)) + position[0], (1 * (scale[1] / 2)) + position[1], (1 * (-scale[2] / 2)) + position[2], color[0], color[1], color[2]);   // +x, +y, -z
        pushVertex((1 * (scale[0] / 2)) + position[0], (1 * (-scale[1] / 2)) + position[1], (1 * (scale[2] / 2)) + position[2], color[0], color[1], color[2]);   // +x, -y, +z
        pushVertex((1 * (-scale[0] / 2)) + position[0], (1 * (-scale[1] / 2)) + position[1], (1 * (scale[2] / 2)) + position[2], color[0], color[1], color[2]);  // -x, -y, +z
    }

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

void createCube(Vector3f position, Vector3f scale) {
    createCube(position, scale, {-1, -1, -1});
}

void createCubeUsingBoundaries(Vector3f position,
                              float x_left_boundary, float x_right_boundary, 
                              float y_up_boundary, float y_down_boundary,
                              float z_front_boundary, float z_back_boundary,
                              Vector3f color) {
    if (color[0] == -1.0f && color[1] == -1.0f && color[2] == -1.0f) {    // color not defined, random colors are used instead
        pushVertex(x_right_boundary + position[0], y_up_boundary + position[1], z_front_boundary + position[2]);   // +x, +y, +z
        pushVertex(x_left_boundary + position[0], y_up_boundary + position[1], z_back_boundary + position[2]);     // -x, +y, -z
        pushVertex(x_left_boundary + position[0], y_up_boundary + position[1], z_front_boundary + position[2]);    // -x, +y, +z
        pushVertex(x_right_boundary + position[0], y_down_boundary + position[1], z_back_boundary + position[2]);  // +x, -y, -z
        pushVertex(x_left_boundary + position[0], y_down_boundary + position[1], z_back_boundary + position[2]);   // -x, -y, -z
        pushVertex(x_right_boundary + position[0], y_up_boundary + position[1], z_back_boundary + position[2]);    // +x, +y, -z
        pushVertex(x_right_boundary + position[0], y_down_boundary + position[1], z_front_boundary + position[2]); // +x, -y, +z
        pushVertex(x_left_boundary + position[0], y_down_boundary + position[1], z_front_boundary + position[2]);  // -x, -y, +z
    }
    else {
        pushVertex(x_right_boundary + position[0], y_up_boundary + position[1], z_front_boundary + position[2], color[0], color[1], color[2]);   // +x, +y, +z
        pushVertex(x_left_boundary + position[0], y_up_boundary + position[1], z_back_boundary + position[2], color[0], color[1], color[2]);     // -x, +y, -z
        pushVertex(x_left_boundary + position[0], y_up_boundary + position[1], z_front_boundary + position[2], color[0], color[1], color[2]);    // -x, +y, +z
        pushVertex(x_right_boundary + position[0], y_down_boundary + position[1], z_back_boundary + position[2], color[0], color[1], color[2]);  // +x, -y, -z
        pushVertex(x_left_boundary + position[0], y_down_boundary + position[1], z_back_boundary + position[2], color[0], color[1], color[2]);   // -x, -y, -z
        pushVertex(x_right_boundary + position[0], y_up_boundary + position[1], z_back_boundary + position[2], color[0], color[1], color[2]);    // +x, +y, -z
        pushVertex(x_right_boundary + position[0], y_down_boundary + position[1], z_front_boundary + position[2], color[0], color[1], color[2]); // +x, -y, +z
        pushVertex(x_left_boundary + position[0], y_down_boundary + position[1], z_front_boundary + position[2], color[0], color[1], color[2]);  // -x, -y, +z
    }

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

    createCubeUsingBoundaries(position,
        x_left_boundary, x_right_boundary,
        y_up_boundary, y_down_boundary,
        z_front_boundary, z_back_boundary, { -1.0f, -1.0f, -1.0f });
}

void createTable(Vector3f position,
                 float table_length, float table_height, float table_width,
                 float tabletop_thickness, float tableleg_length, float tableleg_width,
                 Vector3f color) {
    if (color[0] == -1.0f && color[1] == -1.0f && color[2] == -1.0f) {    // color not defined, random colors are used instead
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
    else {
        // tabletop
        createCube({ position[0], position[1] + table_height - (tabletop_thickness / 2), position[2] },
            { table_length, tabletop_thickness, table_width },
            color);

        // table legs
        // left back
        createCubeUsingBoundaries({ position[0], position[1], position[2] },
            -(table_length / 2), -(table_length / 2) + tableleg_length,
            table_height - tabletop_thickness, -table_height,
            -(table_width / 2) + tableleg_width, -(table_width / 2),
            color);

        // left front
        createCubeUsingBoundaries({ position[0], position[1], position[2] },
            -(table_length / 2), -(table_length / 2) + tableleg_length,
            table_height - tabletop_thickness, -table_height,
            (table_width / 2), (table_width / 2) - tableleg_width,
            color);

        // right back
        createCubeUsingBoundaries({ position[0], position[1], position[2] },
            (table_length / 2) - tableleg_length, (table_length / 2),
            table_height - tabletop_thickness, -table_height,
            -(table_width / 2) + tableleg_width, -(table_width / 2),
            color);

        // right front
        createCubeUsingBoundaries({ position[0], position[1], position[2] },
            (table_length / 2) - tableleg_length, (table_length / 2),
            table_height - tabletop_thickness, -table_height,
            (table_width / 2), (table_width / 2) - tableleg_width,
            color);
    }
}

void createTable(Vector3f position,
    float table_length, float table_height, float table_width,
    float tabletop_thickness, float tableleg_length, float tableleg_width) {

    createTable(position, table_length, table_height, table_width,
        tabletop_thickness, tableleg_length, tableleg_width, { -1.0f, -1.0f, -1.0f });
}

void createRegularIcosahedron(Vector3f position, Vector3f scale, Vector3f color) {
    // those coordinates are pre-calculated
    float x = 0.525731112119133606f;
    float z = 0.850650808352039932f;
    float y = 0.0f;

    if (color[0] == -1.0f && color[1] == -1.0f && color[2] == -1.0f) {    // color not defined, random colors are used instead
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
    }
    else {
        pushVertex(position[0] - (x * scale[0]), position[1] + (y * scale[1]), position[2] + (z * scale[2]), color[0], color[1], color[2]);
        pushVertex(position[0] + (x * scale[0]), position[1] + (y * scale[1]), position[2] + (z * scale[2]), color[0], color[1], color[2]);
        pushVertex(position[0] - (x * scale[0]), position[1] + (y * scale[1]), position[2] - (z * scale[2]), color[0], color[1], color[2]);
        pushVertex(position[0] + (x * scale[0]), position[1] + (y * scale[1]), position[2] - (z * scale[2]), color[0], color[1], color[2]);

        pushVertex(position[0] + (y * scale[0]), position[1] + (z * scale[1]), position[2] + (x * scale[2]), color[0], color[1], color[2]);
        pushVertex(position[0] + (y * scale[0]), position[1] + (z * scale[1]), position[2] - (x * scale[2]), color[0], color[1], color[2]);
        pushVertex(position[0] + (y * scale[0]), position[1] - (z * scale[1]), position[2] + (x * scale[2]), color[0], color[1], color[2]);
        pushVertex(position[0] + (y * scale[0]), position[1] - (z * scale[1]), position[2] - (x * scale[2]), color[0], color[1], color[2]);

        pushVertex(position[0] + (z * scale[0]), position[1] + (x * scale[1]), position[2] + (y * scale[2]), color[0], color[1], color[2]);
        pushVertex(position[0] - (z * scale[0]), position[1] + (x * scale[1]), position[2] + (y * scale[2]), color[0], color[1], color[2]);
        pushVertex(position[0] + (z * scale[0]), position[1] - (x * scale[1]), position[2] + (y * scale[2]), color[0], color[1], color[2]);
        pushVertex(position[0] - (z * scale[0]), position[1] - (x * scale[1]), position[2] + (y * scale[2]), color[0], color[1], color[2]);
    }

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

void createRegularIcosahedron(Vector3f position, Vector3f scale) {
    createRegularIcosahedron(position, scale, { -1.0f, -1.0f, -1.0f });
}

void createSubdividedIcosahedron(int subdiv, Vector3f color) {
    // only works if the regular icosahedron is positioned at origin and is scaled at 1.0
    if (color[0] == -1.0f && color[1] == -1.0f && color[2] == -1.0f) {    // color not defined, random colors are used instead
        createRegularIcosahedron({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
    }
    else {
        createRegularIcosahedron({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, color);
    }

    auto indices_size = Indices.size();
    for (int i = 0; i < subdiv; i++) {
        indices_size = Indices.size();
        for (int triangle = 0; triangle < indices_size; triangle+=3) {
            subdivideTriangle(0);
        }
    }
}

void createSubdividedIcosahedron(int subdiv) {
    createSubdividedIcosahedron(subdiv, { -1.0f, -1.0f, -1.0f });
}

// Debug

void debug_print_vertices(vector<Vertex> &ver) {
    cout << "\n*** DEBUG VERTICES ***\n";
    cout << "ver.size() = " << ver.size() << "\n";
    cout << "sizeof(ver[0]) = " << sizeof(ver[0]) << "\n";
    cout << "ver.size() * sizeof(ver[0]) = " << ver.size() * sizeof(ver[0]) << "\n";
    for (int i = 0; i < ver.size(); i++) {
        cout << "pos: { " << ver[i].pos[0] << " " << ver[i].pos[1] << " " << ver[i].pos[2] << " }\n";
        cout << "color: { " << ver[i].color[0] << " " << ver[i].color[1] << " " << ver[i].color[2] << " }\n";
    }
}

void debug_print_indices(vector<unsigned int> &ind) {
    cout << "\n*** DEBUG INDICES ***\n";
    cout << "ind.size() = " << ind.size() << "\n";
    cout << "sizeof(ind[0]) = " << sizeof(ind[0]) << "\n";
    cout << "ind.size() * sizeof(ind[0]) = " << ind.size() * sizeof(ind[0]) << "\n";

    int j = 0;
    for (int i = 0; i < ind.size(); i++) {
        cout << ind[i] << "  ";
        j++;
        if (j > 2) {
            cout << "\n";
            j = 0;
        }
    }
}