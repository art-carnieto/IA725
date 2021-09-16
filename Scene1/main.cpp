/*

        Copyright 2010 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Tutorial 13 - Camera Space
*/

#include <stdio.h>
#include <string.h>

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogldev_util.h"
#include "ogldev_math_3d.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

// to fix 'M_PI' not declared error
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLuint VBO;
GLuint IBO;
GLuint gWVPLocation;


#define NFACE 20
#define NVERTEX 12

#define X .525731112119133606 
#define Z .850650808352039932

// These are the 12 vertices for the icosahedron
static GLfloat vdata[NVERTEX][3] = {
   {-X, 0.0, Z},{X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

// These are the 20 faces.  Each of the three entries for each 
// vertex gives the 3 vertices that make the face.
static GLint tindices[NFACE][3] = {
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

//Helpers
void normalize(float v[3]) {
    float d = sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
    if (d == 0.0) return;
    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

void drawTriangle(GLfloat* v1, GLfloat* v2, GLfloat* v3)
{
    // Draw a triangle with specified vertices 
    glBegin(GL_TRIANGLES);
    //  glColor3f(0.0,0.0,0.8);
    glColor3f(rand() / ((float)RAND_MAX + 1), rand() / ((float)RAND_MAX + 1), rand() / ((float)RAND_MAX + 1));
    //  glNormal3fv(v1); 
    //  glNormal3fv(v2); 
    //  glNormal3fv(v3);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(1.0, 1.0, 1.0);
    //  glNormal3fv(v1); 
    //  glNormal3fv(v2); 
    //  glNormal3fv(v3);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glEnd();
}

void subDivide(GLfloat* v1, GLfloat* v2, GLfloat* v3, int depth) {
    if (depth == 0) {
        drawTriangle(v1, v2, v3);
        return;
    }
    //midpoint of each edge
    GLfloat v12[3];
    GLfloat v23[3];
    GLfloat v31[3];
    for (int i = 0; i < 3; i++) {
        v12[i] = v1[i] + v2[i];
        v23[i] = v2[i] + v3[i];
        v31[i] = v3[i] + v1[i];
    }
    normalize(v12);
    normalize(v23);
    normalize(v31);

    subDivide(v1, v12, v31, depth - 1);
    subDivide(v2, v23, v12, depth - 1);
    subDivide(v3, v31, v23, depth - 1);
    subDivide(v12, v23, v31, depth - 1);
}

void drawIcosahedron(int maxDepth) {
    for (int i = 0; i < NFACE; i++) {
        srand(i);
        subDivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], maxDepth);
    }
}

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    //glutSolidTeapot(1);

    static float Scale = 0.0f;

#ifdef _WIN64
    Scale += 0.001f;
#else
    Scale += 0.02f;
#endif

    Matrix4f Rotation(
        cosf(Scale), 0.0f, -sinf(Scale), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinf(Scale), 0.0f, cosf(Scale), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    Matrix4f Translation(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    Matrix4f World = Translation * Rotation;

    Vector3f CameraPos(0.0f, 0.0f, -6.0f);
    Vector3f U(1.0f, 0.0f, 0.0f);
    Vector3f V(0.0f, 1.0f, 0.0f);
    Vector3f N(0.0f, 0.0f, 1.0f);

    Matrix4f Camera(
        U.x, U.y, U.z, -CameraPos.x,
        V.x, V.y, V.z, -CameraPos.y,
        N.x, N.y, N.z, -CameraPos.z,
        0.0f, 0.0f, 0.0f, 1.0f);

    float VFOV = 45.0f;
    float tanHalfVFOV = tanf(ToRadian(VFOV / 2.0f));
    float d = 1 / tanHalfVFOV;
    float ar = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

    float NearZ = 1.0f;
    float FarZ = 10.0f;

    float zRange = NearZ - FarZ;

    float A = (-FarZ - NearZ) / zRange;
    float B = 2.0f * FarZ * NearZ / zRange;

    Matrix4f Projection(
        d / ar, 0.0f, 0.0f, 0.0f,
        0.0f, d, 0.0f, 0.0f,
        0.0f, 0.0f, A, B,
        0.0f, 0.0f, 1.0f, 0.0f);

    Matrix4f WVP = Projection * Camera * World;

    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, &WVP.m[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glDrawElements(GL_TRIANGLES, (36 * 5), GL_UNSIGNED_INT, 0); // draw 36 indices times 5 objects (rectangular cuboids)

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    drawIcosahedron(4);

    glutPostRedisplay();

    glutSwapBuffers();
}


struct Vertex {
    Vector3f pos;
    Vector3f color;

    Vertex() {}

    Vertex(float x, float y, float z)
    {
        pos = Vector3f(x, y, z);

        float red = (float)rand() / (float)RAND_MAX;
        float green = (float)rand() / (float)RAND_MAX;
        float blue = (float)rand() / (float)RAND_MAX;
        color = Vector3f(red, green, blue);
    }
};


static void CreateVertexBuffer()
{
    Vertex Vertices[8 * 5]; // 8 times 5 rectangular cuboids

    const float table_posY = -1.5f; // table object position

    // tabletop size
    const float tabletop_y = 0.03f;
    const float tabletop_x = 2.0f;
    const float tabletop_z = 2.0f;

    // table legs size
    const float table_leg_y = 0.5f;
    const float table_leg_x = 0.05f;
    const float table_leg_z = 0.05f;

    const float tabletop_posY = 0.5f; // makes the tabletop go up

    // table legs positions
    const float table_leg1_posX = -2.0f + table_leg_x;
    const float table_leg1_posZ = -2.0f + table_leg_z;

    const float table_leg2_posX = 2.0f - table_leg_x;
    const float table_leg2_posZ = -2.0f + table_leg_z;

    const float table_leg3_posX = -2.0f + table_leg_x;
    const float table_leg3_posZ = 2.0f - table_leg_z;

    const float table_leg4_posX = 2.0f - table_leg_x;
    const float table_leg4_posZ = 2.0f - table_leg_z;

    // tabletop
    Vertices[0] = Vertex(tabletop_x, table_posY + tabletop_y + tabletop_posY, tabletop_z);
    Vertices[1] = Vertex(-tabletop_x, table_posY + tabletop_y + tabletop_posY, -tabletop_z);
    Vertices[2] = Vertex(-tabletop_x, table_posY + tabletop_y + tabletop_posY, tabletop_z);
    Vertices[3] = Vertex(tabletop_x, table_posY + -tabletop_y + tabletop_posY, -tabletop_z);
    Vertices[4] = Vertex(-tabletop_x, table_posY + -tabletop_y + tabletop_posY, -tabletop_z);
    Vertices[5] = Vertex(tabletop_x, table_posY + tabletop_y + tabletop_posY, -tabletop_z);
    Vertices[6] = Vertex(tabletop_x, table_posY + -tabletop_y + tabletop_posY, tabletop_z);
    Vertices[7] = Vertex(-tabletop_x, table_posY + -tabletop_y + tabletop_posY, tabletop_z);

    // table leg 1
    Vertices[8] = Vertex(table_leg_x + table_leg1_posX, table_posY + table_leg_y, table_leg_z + table_leg1_posZ);
    Vertices[9] = Vertex(-table_leg_x + table_leg1_posX, table_posY + table_leg_y, -table_leg_z + table_leg1_posZ);
    Vertices[10] = Vertex(-table_leg_x + table_leg1_posX, table_posY + table_leg_y, table_leg_z + table_leg1_posZ);
    Vertices[11] = Vertex(table_leg_x + table_leg1_posX, table_posY + -table_leg_y, -table_leg_z + table_leg1_posZ);
    Vertices[12] = Vertex(-table_leg_x + table_leg1_posX, table_posY + -table_leg_y, -table_leg_z + table_leg1_posZ);
    Vertices[13] = Vertex(table_leg_x + table_leg1_posX, table_posY + table_leg_y, -table_leg_z + table_leg1_posZ);
    Vertices[14] = Vertex(table_leg_x + table_leg1_posX, table_posY + -table_leg_y, table_leg_z + table_leg1_posZ);
    Vertices[15] = Vertex(-table_leg_x + table_leg1_posX, table_posY + -table_leg_y, table_leg_z + table_leg1_posZ);

    // table leg 2
    Vertices[16] = Vertex(table_leg_x + table_leg2_posX, table_posY + table_leg_y, table_leg_z + table_leg2_posZ);
    Vertices[17] = Vertex(-table_leg_x + table_leg2_posX, table_posY + table_leg_y, -table_leg_z + table_leg2_posZ);
    Vertices[18] = Vertex(-table_leg_x + table_leg2_posX, table_posY + table_leg_y, table_leg_z + table_leg2_posZ);
    Vertices[19] = Vertex(table_leg_x + table_leg2_posX, table_posY + -table_leg_y, -table_leg_z + table_leg2_posZ);
    Vertices[20] = Vertex(-table_leg_x + table_leg2_posX, table_posY + -table_leg_y, -table_leg_z + table_leg2_posZ);
    Vertices[21] = Vertex(table_leg_x + table_leg2_posX, table_posY + table_leg_y, -table_leg_z + table_leg2_posZ);
    Vertices[22] = Vertex(table_leg_x + table_leg2_posX, table_posY + -table_leg_y, table_leg_z + table_leg2_posZ);
    Vertices[23] = Vertex(-table_leg_x + table_leg2_posX, table_posY + -table_leg_y, table_leg_z + table_leg2_posZ);

    // table leg 3
    Vertices[24] = Vertex(table_leg_x + table_leg3_posX, table_posY + table_leg_y, table_leg_z + table_leg3_posZ);
    Vertices[25] = Vertex(-table_leg_x + table_leg3_posX, table_posY + table_leg_y, -table_leg_z + table_leg3_posZ);
    Vertices[26] = Vertex(-table_leg_x + table_leg3_posX, table_posY + table_leg_y, table_leg_z + table_leg3_posZ);
    Vertices[27] = Vertex(table_leg_x + table_leg3_posX, table_posY + -table_leg_y, -table_leg_z + table_leg3_posZ);
    Vertices[28] = Vertex(-table_leg_x + table_leg3_posX, table_posY + -table_leg_y, -table_leg_z + table_leg3_posZ);
    Vertices[29] = Vertex(table_leg_x + table_leg3_posX, table_posY + table_leg_y, -table_leg_z + table_leg3_posZ);
    Vertices[30] = Vertex(table_leg_x + table_leg3_posX, table_posY + -table_leg_y, table_leg_z + table_leg3_posZ);
    Vertices[31] = Vertex(-table_leg_x + table_leg3_posX, table_posY + -table_leg_y, table_leg_z + table_leg3_posZ);

    // table leg 4
    Vertices[32] = Vertex(table_leg_x + table_leg4_posX, table_posY + table_leg_y, table_leg_z + table_leg4_posZ);
    Vertices[33] = Vertex(-table_leg_x + table_leg4_posX, table_posY + table_leg_y, -table_leg_z + table_leg4_posZ);
    Vertices[34] = Vertex(-table_leg_x + table_leg4_posX, table_posY + table_leg_y, table_leg_z + table_leg4_posZ);
    Vertices[35] = Vertex(table_leg_x + table_leg4_posX, table_posY + -table_leg_y, -table_leg_z + table_leg4_posZ);
    Vertices[36] = Vertex(-table_leg_x + table_leg4_posX, table_posY + -table_leg_y, -table_leg_z + table_leg4_posZ);
    Vertices[37] = Vertex(table_leg_x + table_leg4_posX, table_posY + table_leg_y, -table_leg_z + table_leg4_posZ);
    Vertices[38] = Vertex(table_leg_x + table_leg4_posX, table_posY + -table_leg_y, table_leg_z + table_leg4_posZ);
    Vertices[39] = Vertex(-table_leg_x + table_leg4_posX, table_posY + -table_leg_y, table_leg_z + table_leg4_posZ);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
    unsigned int Indices[] = {
        // tabletop
        0, 1, 2,
        1, 3, 4,
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1,
        1, 5, 3,
        5, 0, 6,
        7, 4, 3,
        2, 1, 4,
        0, 2, 7,

        // table leg 1
        8, 9, 10,
        9, 11, 12,
        13, 14, 11,
        15, 11, 14,
        10, 12, 15,
        8, 15, 14,
        8, 13, 9,
        9, 13, 11,
        13, 8, 14,
        15, 12, 11,
        10, 9, 12,
        8, 10, 15,

        // table leg 2
        16, 17, 18,
        17, 19, 20,
        21, 22, 19,
        23, 19, 22,
        18, 20, 23,
        16, 23, 22,
        16, 21, 17,
        17, 21, 19,
        21, 16, 22,
        23, 20, 19,
        18, 17, 20,
        16, 18, 23,

        // table leg 3
        24, 25, 26,
        25, 27, 28,
        29, 30, 27,
        31, 27, 30,
        26, 28, 31,
        24, 31, 30,
        24, 29, 25,
        25, 29, 27,
        29, 24, 30,
        31, 28, 27,
        26, 25, 28,
        24, 26, 31,

        // table leg 4
        32, 33, 34,
        33, 35, 36,
        37, 38, 35,
        39, 35, 38,
        34, 36, 39,
        32, 39, 38,
        32, 37, 33,
        33, 37, 35,
        37, 32, 38,
        39, 36, 35,
        34, 33, 36,
        32, 34, 39
    };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(1);
    }

    const GLchar* p[1];
    p[0] = pShaderText;

    GLint Lengths[1];
    Lengths[0] = (GLint)strlen(pShaderText);

    glShaderSource(ShaderObj, 1, p, Lengths);

    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    std::string vs, fs;

    if (!ReadFile(pVSFileName, vs)) {
        exit(1);
    };

    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    if (!ReadFile(pFSFileName, fs)) {
        exit(1);
    };

    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    if (gWVPLocation == -1) {
        printf("Error getting uniform location of 'gWVP'\n");
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);
}

int main(int argc, char** argv)
{
#ifdef _WIN64
    srand(GetCurrentProcessId());
#else
    srandom(getpid());
#endif

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    int x = 200;
    int y = 100;
    glutInitWindowPosition(x, y);
    int win = glutCreateWindow("Scene1");
    printf("window id: %d\n", win);

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode

    CreateVertexBuffer();
    CreateIndexBuffer();

    CompileShaders();

    glutDisplayFunc(RenderSceneCB);

    glutMainLoop();

    return 0;
}
