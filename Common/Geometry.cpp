#include "Geometry.hpp"
#include <vector>
#include "TeapotData.hpp"

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

// compare if two floats are equals, taken from: https://www.tutorialspoint.com/floating-point-comparison-in-cplusplus
bool cmp_eq_float(float x, float y, float epsilon = 0.001f) {
    if (fabs(x - y) < epsilon)
        return true;  //they are same
    return false;     //they are not same
}

// adapted from http://www.inf.ufsc.br/~aldo.vw/grafica/apostilas/openGL/lesson29/lesson29.html
// Calculates 3rd Degree Polynomial Based On Array Of 4 Points
// And A Single Variable (u) Which Is Generally Between 0 And 1
Vertex BernsteinCurve(float u, Vertex points[4], Vector3f color) {
    float a[3], b[3], c[3], d[3];  // 4 control points

    for (int i = 0; i < 3; i++) {  // access the x, y and z dimensions
        a[i] = static_cast<float>(pow((1 - u), 3));
        b[i] = static_cast<float>(3 * u * pow((1 - u), 2));
        c[i] = static_cast<float>(3 * pow(u, 2) * (1 - u));
        d[i] = static_cast<float>(pow(u, 3));
    }

    float bernstein[3];  // x, y and z
    for (int i = 0; i < 3; i++) {  // access the x, y and z dimensions
        bernstein[i] = points[0].getPosition()[i] * a[i] + 
            points[1].getPosition()[i] * b[i] +
            points[2].getPosition()[i] * c[i] +
            points[3].getPosition()[i] * d[i];
    }
    Vertex new_vertex = Vertex(bernstein[0], bernstein[1], bernstein[2], color[0], color[1], color[2]);
    
    return new_vertex;
}

// adapted from https://github.com/rgalo-coder/ComputacaoGrafica/blob/master/ExercicioBase/BuleUtah.cpp
Vertex BernsteinSurface(float u, float v, Vertex patch_points[4][4], Vector3f color) {
    Vertex uCurve[4];
    for (int i = 0; i < 4; i++) {  // gets each column of the 4x4 grid separately
        uCurve[i] = BernsteinCurve(u, patch_points[i], color);
    }
    return BernsteinCurve(v, uCurve, color);
}

vector<Vertex> genPatchBezier(Vertex patch_points[4][4], int divs, Vector3f color) {
    vector<Vertex> patch;
    
    float step = 1.0f / divs;
    float u = 0.0f;
    float v = 0.0f;
    
    for (int i = 0; i <= divs; i++) {
        for (int j = 0; j <= divs; j++) {
            patch.emplace_back(BernsteinSurface(u, v, patch_points, color));
            u += step;
        }
        u = 0.0f;
        v += step;
    }
    return patch;
}

vector<Vertex> genPatchBezierUsingIndices(vector<Vertex> list_control_points, unsigned int indices[16], int divs, Vector3f color) {
    Vertex patch_points[4][4];

    patch_points[0][0] = list_control_points[indices[0]];
    patch_points[0][1] = list_control_points[indices[1]];
    patch_points[0][2] = list_control_points[indices[2]];
    patch_points[0][3] = list_control_points[indices[3]];

    patch_points[1][0] = list_control_points[indices[4]];
    patch_points[1][1] = list_control_points[indices[5]];
    patch_points[1][2] = list_control_points[indices[6]];
    patch_points[1][3] = list_control_points[indices[7]];
    
    patch_points[2][0] = list_control_points[indices[8]];
    patch_points[2][1] = list_control_points[indices[9]];
    patch_points[2][2] = list_control_points[indices[10]];
    patch_points[2][3] = list_control_points[indices[11]];
    
    patch_points[3][0] = list_control_points[indices[12]];
    patch_points[3][1] = list_control_points[indices[13]];
    patch_points[3][2] = list_control_points[indices[14]];
    patch_points[3][3] = list_control_points[indices[15]];
    
    return genPatchBezier(patch_points, divs, color);
}

vector<Vertex> loadTeapotVertices() {
    vector<Vertex> teapot;
    for (int i = 0; i < teapot_number_vertices; i++) {
        teapot.emplace_back(Vertex(teapot_vertices[i][0], teapot_vertices[i][1], teapot_vertices[i][2]));
    }

    return teapot;
}

vector<unsigned int> loadTeapotIndices() {
    vector<unsigned int> teapot;
    for (int i = 0; i < teapot_number_indices; i++) {
        for (int j = 0; j < 16; j++) {  // 16 control points each
            teapot.emplace_back(teapot_indices[i][j] - 1);  // -1 because the indices start at 1, and not 0
        }
    }

    return teapot;
}

// Object creation

Mesh createCube(Vector3f color) {
    Mesh m;
    m.setUsesIndices(false);  // cube will not use indices to have correct normal vectors on each side

    m.pushVertex({ 0.5f, 0.5f, 0.5f }, color, { 0.0f, 1.0f, 0.0f });    // 0
    m.pushVertex({ -0.5f, 0.5f, -0.5f }, color, { 0.0f, 1.0f, 0.0f });  // 1
    m.pushVertex({ -0.5f, 0.5f, 0.5f }, color, { 0.0f, 1.0f, 0.0f });   // 2

    m.pushVertex({ -0.5f, 0.5f, -0.5f }, color, { 0.0f, 0.0f, -1.0f });  // 1
    m.pushVertex({ 0.5f, -0.5f, -0.5f }, color, { 0.0f, 0.0f, -1.0f });  // 3
    m.pushVertex({ -0.5f, -0.5f, -0.5f }, color, { 0.0f, 0.0f, -1.0f }); // 4

    m.pushVertex({ 0.5f, 0.5f, -0.5f }, color, { 1.0f, 0.0f, 0.0f });   // 5
    m.pushVertex({ 0.5f, -0.5f, 0.5f }, color, { 1.0f, 0.0f, 0.0f });   // 6
    m.pushVertex({ 0.5f, -0.5f, -0.5f }, color, { 1.0f, 0.0f, 0.0f });  // 3
        
    m.pushVertex({ -0.5f, -0.5f, 0.5f }, color, { 0.0f, -1.0f, 0.0f });  // 7
    m.pushVertex({ 0.5f, -0.5f, -0.5f }, color, { 0.0f, -1.0f, 0.0f });  // 3
    m.pushVertex({ 0.5f, -0.5f, 0.5f }, color, { 0.0f, -1.0f, 0.0f });   // 6

    m.pushVertex({ -0.5f, 0.5f, 0.5f }, color, { -1.0f, 0.0f, 0.0f });   // 2
    m.pushVertex({ -0.5f, -0.5f, -0.5f }, color, { -1.0f, 0.0f, 0.0f }); // 4
    m.pushVertex({ -0.5f, -0.5f, 0.5f }, color, { -1.0f, 0.0f, 0.0f });  // 7

    m.pushVertex({ 0.5f, 0.5f, 0.5f }, color, { 0.0f, 0.0f, 1.0f });    // 0
    m.pushVertex({ -0.5f, -0.5f, 0.5f }, color, { 0.0f, 0.0f, 1.0f });  // 7
    m.pushVertex({ 0.5f, -0.5f, 0.5f }, color, { 0.0f, 0.0f, 1.0f });   // 6

    m.pushVertex({ 0.5f, 0.5f, 0.5f }, color, { 0.0f, 1.0f, 0.0f });    // 0
    m.pushVertex({ 0.5f, 0.5f, -0.5f }, color, { 0.0f, 1.0f, 0.0f });   // 5
    m.pushVertex({ -0.5f, 0.5f, -0.5f }, color, { 0.0f, 1.0f, 0.0f });  // 1

    m.pushVertex({ -0.5f, 0.5f, -0.5f }, color, { 0.0f, 0.0f, -1.0f });  // 1
    m.pushVertex({ 0.5f, 0.5f, -0.5f }, color, { 0.0f, 0.0f, -1.0f });   // 5
    m.pushVertex({ 0.5f, -0.5f, -0.5f }, color, { 0.0f, 0.0f, -1.0f });  // 3

    m.pushVertex({ 0.5f, 0.5f, -0.5f }, color, { 1.0f, 0.0f, 0.0f });   // 5
    m.pushVertex({ 0.5f, 0.5f, 0.5f }, color, { 1.0f, 0.0f, 0.0f });    // 0
    m.pushVertex({ 0.5f, -0.5f, 0.5f }, color, { 1.0f, 0.0f, 0.0f });   // 6

    m.pushVertex({ -0.5f, -0.5f, 0.5f }, color, { 0.0f, -1.0f, 0.0f });  // 7
    m.pushVertex({ -0.5f, -0.5f, -0.5f }, color, { 0.0f, -1.0f, 0.0f }); // 4
    m.pushVertex({ 0.5f, -0.5f, -0.5f }, color, { 0.0f, -1.0f, 0.0f });  // 3

    m.pushVertex({ -0.5f, 0.5f, 0.5f }, color, { -1.0f, 0.0f, 0.0f });   // 2
    m.pushVertex({ -0.5f, 0.5f, -0.5f }, color, { -1.0f, 0.0f, 0.0f });  // 1
    m.pushVertex({ -0.5f, -0.5f, -0.5f }, color, { -1.0f, 0.0f, 0.0f }); // 4

    m.pushVertex({ 0.5f, 0.5f, 0.5f }, color, { 0.0f, 0.0f, 1.0f });    // 0
    m.pushVertex({ -0.5f, 0.5f, 0.5f }, color, { 0.0f, 0.0f, 1.0f });   // 2
    m.pushVertex({ -0.5f, -0.5f, 0.5f }, color, { 0.0f, 0.0f, 1.0f });  // 7
    
    return m;
}

Mesh createTable(Vector3f position,
    float table_length, float table_height, float table_width,
    float tabletop_thickness, float tableleg_length, float tableleg_width,
    Vector3f color) {

    // this is the cube primitive that will be transformed 5 times to create a table
    Mesh cube_primitive = createCube(color);

    // tabletop
    Transformation t_tabletop = Transformation();
    t_tabletop.setTranslation({ 0.0f + position[0], table_height + position[1], 0.0f + position[2] });
    t_tabletop.setScale({ table_length, tabletop_thickness, table_width });
    cube_primitive.pushTransformation(t_tabletop);

    // table leg 1: left back
    Transformation t_table_leg_1 = Transformation();
    t_table_leg_1.setTranslation({ (-table_length / 2) + (tableleg_length / 2) + position[0], 0.0f + position[1], (-table_width / 2) + (tableleg_width / 2) + position[2] });
    t_table_leg_1.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    cube_primitive.pushTransformation(t_table_leg_1);

    // table leg 2: left front
    Transformation t_table_leg_2 = Transformation();
    t_table_leg_2.setTranslation({ (-table_length / 2) + (tableleg_length / 2) + position[0], 0.0f + position[1], (+table_width / 2) - (tableleg_width / 2) + position[2] });
    t_table_leg_2.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    cube_primitive.pushTransformation(t_table_leg_2);

    // table leg 3: right back
    Transformation t_table_leg_3 = Transformation();
    t_table_leg_3.setTranslation({ (+table_length / 2) - (tableleg_length / 2) + position[0], 0.0f + position[1], (-table_width / 2) + (tableleg_width / 2) + position[2] });
    t_table_leg_3.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    cube_primitive.pushTransformation(t_table_leg_3);

    // table leg 4: right front
    Transformation t_table_leg_4 = Transformation();
    t_table_leg_4.setTranslation({ (+table_length / 2) - (tableleg_length / 2) + position[0], 0.0f + position[1], (+table_width / 2) - (tableleg_width / 2) + position[2] });
    t_table_leg_4.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    cube_primitive.pushTransformation(t_table_leg_4);

    return cube_primitive;
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

Mesh createPlane(float delta, Vector3f color) {
    // FIXME: if delta value is <= 0.05 the indices are calculated wrong!
    // This happens because of floating operation precision errors in u = u + delta and v = v + delta.
    // Try with delta = 0.05: it won't create the vertices in x = 0.5 and z = 0.5 (last row and column)
    float epsilon = 0.05f; // floating point precision error tolerance
    assert(delta > epsilon);

    Mesh m;

    float size_u = 1.0f;
    float size_v = 1.0f;

    float u_start = -0.5f;
    float v_start = -0.5f;

    // Create vertices
    for (float u = u_start; u <= (u_start + size_u); u = u + delta) {
        for (float v = v_start; v <= (v_start + size_v); v = v + delta) {
            m.pushVertex({ u, 0.0f, v }, color);
        }
    }
    
    // Create indices
    int num_points_col = static_cast<int>(size_u / delta) + 1;
    int index = 0;
    for (int i = 0; i < num_points_col; i++) {
        for (int j = 0; j < (num_points_col - 1); j++) {
            if ((index + 1) % num_points_col == 0) {
                index++;
                continue;
            }
            m.pushTriangleIndices(index, index + 1, index + num_points_col);
            m.pushTriangleIndices(index + 1, index + num_points_col + 1, index + num_points_col);
            index++;
        }
    }

    return m;
}

Mesh createCircle(int subdiv, float radius, Vector3f color) {
    Mesh m;
    m.pushVertex({ 0.0f, 0.0f, 0.0f }, color);  // midpoint

    float theta_step = 360.0f / subdiv;
    float theta = 0.0f;

    // Vertices
    for (int i = 0; i < subdiv; i++) {
        m.pushVertex({ radius * cos(ToRadian(theta)), 0.0f, radius * sin(ToRadian(theta)) }, color);
        theta += theta_step;
    }
    // Indices
    for (int i = 1; i < subdiv; i++) {
        m.pushTriangleIndices(i, 0, i + 1);  // circle is facing up
    }
    m.pushTriangleIndices(subdiv, 0, 1);  // last triangle

    return m;
}

Mesh createCone(int subdiv, float radius, float height, Vector3f color) {
    Mesh m;

    float theta_step = 360.0f / subdiv;
    float theta = 0.0f;

    // Circular base
    // Vertices
    m.pushVertex({ 0.0f, 0.0f, 0.0f }, color);  // midpoint
    for (int i = 0; i < subdiv; i++) {
        m.pushVertex({ radius * cos(ToRadian(theta)), 0.0f, radius * sin(ToRadian(theta)) }, color);
        theta += theta_step;
    }
    // Indices
    for (int i = 1; i < subdiv; i++) {
        m.pushTriangleIndices(0, i, i + 1);  // circle is facing down
    }
    m.pushTriangleIndices(0, subdiv, 1);  // last triangle of base

    // Body
    m.pushVertex({ 0.0f, height, 0.0f }, color);  // top vertex
    // Indices
    for (int i = 1; i < subdiv; i++) {
        m.pushTriangleIndices(i, subdiv + 1, i + 1);
    }
    m.pushTriangleIndices(subdiv, subdiv + 1, 1);  // last triangle of body

    return m;
}

Mesh createCylinder(int subdiv, float radius, float height, Vector3f color) {
    Mesh m;

    float theta_step = 360.0f / subdiv;
    float theta = 0.0f;

    // Circular bottom
    // Vertices
    m.pushVertex({ 0.0f, 0.0f, 0.0f }, color);  // midpoint
    for (int i = 0; i < subdiv; i++) {
        m.pushVertex({ radius * cos(ToRadian(theta)), 0.0f, radius * sin(ToRadian(theta)) }, color);
        theta += theta_step;
    }
    // Indices
    for (int i = 1; i < subdiv; i++) {
        m.pushTriangleIndices(0, i, i + 1);  // circle is facing down
    }
    m.pushTriangleIndices(0, subdiv, 1);  // last triangle of base

    // Circular top
    m.pushVertex({ 0.0f, height, 0.0f }, color);  // top midpoint
    theta = 0.0f;
    // Vertices
    for (int i = 0; i < subdiv; i++) {
        m.pushVertex({ radius * cos(ToRadian(theta)), height, radius * sin(ToRadian(theta)) }, color);
        theta += theta_step;
    }
    // Indices
    for (int i = subdiv + 2; i < (subdiv * 2) + 1; i++) {
        m.pushTriangleIndices(i, subdiv + 1, i + 1);
    }
    m.pushTriangleIndices((subdiv * 2) + 1, subdiv + 1, subdiv + 2);  // last triangle of top circle

    // Body indices
    for (int i = 1; i < subdiv; i++) {
        m.pushTriangleIndices(i, i + subdiv + 1, i + 1);
        m.pushTriangleIndices(i + 1, i + subdiv + 1, i + subdiv + 2);
    }
    m.pushTriangleIndices(subdiv, (subdiv * 2) + 1, 1);
    m.pushTriangleIndices(1, (subdiv * 2) + 1, subdiv + 2);
    return m;
}

// adapted from https://github.com/JoeyDeVries/Cell/blob/master/cell/mesh/sphere.cpp
Mesh createSphere(int xSegments, int ySegments, float radius, Vector3f color) {
    Mesh m;

    for (unsigned int y = 0; y <= ySegments; ++y) {
        for (unsigned int x = 0; x <= xSegments; ++x) {
            float xSegment = (float)x / (float)xSegments;
            float ySegment = (float)y / (float)ySegments;
            float xPos = radius * std::cos(xSegment * (M_PI * 2)) * std::sin(ySegment * M_PI);
            float yPos = radius * std::cos(ySegment * M_PI);
            float zPos = radius * std::sin(xSegment * (M_PI * 2)) * std::sin(ySegment * M_PI);

            m.pushVertex({ xPos, yPos, zPos }, color);
        }
    }

    for (int y = 0; y < ySegments; ++y) {
        for (int x = 0; x < xSegments; ++x) {
            m.pushTriangleIndices(
                (y + 1) * (xSegments + 1) + x,
                y * (xSegments + 1) + x,
                y * (xSegments + 1) + x + 1);

            m.pushTriangleIndices(
                (y + 1) * (xSegments + 1) + x,
                y * (xSegments + 1) + x + 1,
                (y + 1) * (xSegments + 1) + x + 1);
        }
    }

    return m;
}

Mesh createCubicBezierMesh(vector<Vertex> list_control_points, vector<unsigned int> bezier_indices, int subdiv, Vector3f color) {
    // list_control_points should already have the color set
    Mesh m;

    int size_list = bezier_indices.size() / 16;
    int offset = 0;
    vector<unsigned int>::iterator it = bezier_indices.begin();  // indices iterator

    for (int patch_index = 0; patch_index < size_list; patch_index++) {
        unsigned int indices[16];
        for (int i = 0; i < 16; i++) {
            indices[i] = *it;
            it++;
        }

        // create vertices for a single Bezier patch
        vector<Vertex> vertices = genPatchBezierUsingIndices(list_control_points, indices, subdiv, color);

        // push vertices into Mesh
        for (int j = 0; j < vertices.size(); j++) m.pushVertex(vertices[j]);

        // Indices
        // adapted from https://github.com/rgalo-coder/ComputacaoGrafica/blob/master/ExercicioBase/BuleUtah.cpp
        for (int row = 0; row < subdiv; row++) {
            for (int col = 0; col < subdiv; col++) {
                m.pushTriangleIndices(
                    (subdiv + 1) * row + col + offset,
                    (subdiv + 1) * row + col + 1 + offset,
                    (subdiv + 1) * (row + 1) + col + offset
                );
                m.pushTriangleIndices(
                    (subdiv + 1) * (row + 1) + col + 1 + offset,
                    (subdiv + 1) * (row + 1) + col + offset,
                    (subdiv + 1) * row + col + 1 + offset
                );
            }
        }
        offset += vertices.size();
    }

    return m;
}

Mesh createUtahTeapot(int subdiv, Vector3f color) {
    vector<Vertex> teapot_control_points = loadTeapotVertices();
    vector<unsigned int> teapot_indices = loadTeapotIndices();
    return createCubicBezierMesh(teapot_control_points, teapot_indices, subdiv, color);
}