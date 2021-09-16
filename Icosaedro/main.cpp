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
   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
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

    static float Scale = 0.0f;

#ifdef _WIN64
    Scale += 0.001f;
#else
    Scale += 0.02f;
#endif

    Matrix4f Rotation(
        cosf(Scale), 0.0f, -sinf(Scale), 0.0f,
        0.0f,        1.0f, 0.0f,         0.0f,
        sinf(Scale), 0.0f, cosf(Scale),  0.0f,
        0.0f,        0.0f, 0.0f,         1.0f);

    Matrix4f Translation(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    Matrix4f World = Translation * Rotation;

    Vector3f CameraPos(0.0f, 0.0f, -3.0f);
    Vector3f U(1.0f, 0.0f, 0.0f);
    Vector3f V(0.0f, 1.0f, 0.0f);
    Vector3f N(0.0f, 0.0f, 1.0f);

    Matrix4f Camera(
        U.x,  U.y,  U.z,  -CameraPos.x,
        V.x,  V.y,  V.z,  -CameraPos.y,
        N.x,  N.y,  N.z,  -CameraPos.z,
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
        0.0f,   d,    0.0f, 0.0f,
        0.0f,   0.0f, A,    B,
        0.0f,   0.0f, 1.0f, 0.0f);

    Matrix4f WVP = Projection * Camera * World;

    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, &WVP.m[0][0]);

    drawIcosahedron(3); // draw icosahedron of order 3

    glutPostRedisplay();

    glutSwapBuffers();
}

/*
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
    const float posX = 0.525731112119133606f;
    const float posY = 0.0f;
    const float posZ = 0.850650808352039932f;

    Vertex Vertices[12];

    Vertices[0] = Vertex(-posX, 0.0f,  posZ);
    Vertices[1] = Vertex( posX, 0.0f,  posZ);
    Vertices[2] = Vertex(-posX, 0.0f, -posZ);
    Vertices[3] = Vertex( posX, 0.0f, -posZ);

    Vertices[4] = Vertex( 0.0f, posZ,  posX);
    Vertices[5] = Vertex( 0.0f, posZ, -posX);
    Vertices[6] = Vertex( 0.0f,-posZ,  posX);
    Vertices[7] = Vertex( 0.0f,-posZ, -posX);

    Vertices[8] =  Vertex( posZ, posX, 0.0f);
    Vertices[9] =  Vertex(-posZ, posX, 0.0f);
    Vertices[10] = Vertex( posZ,-posX, 0.0f);
    Vertices[11] = Vertex(-posZ,-posX, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}


static void CreateIndexBuffer()
{
    unsigned int Indices[] = {
                              0, 4, 1,
                              0, 9, 4,
                              9, 5, 4,
                              4, 5, 8,
                              4, 8, 1,
                              
                              8, 10, 1,
                              8, 3, 10,
                              5, 3, 8,
                              5, 2, 3,
                              2, 7, 3,

                              7, 10, 3,
                              7, 6, 10,
                              7, 11, 6,
                              11, 0, 6,
                              0, 1, 6,

                              6, 1, 10,
                              9, 0, 11,
                              9, 11, 2,
                              9, 2, 5,
                              7, 2, 11
    };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}
*/

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
    int win = glutCreateWindow("Icosaedro");
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

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode

    //CreateVertexBuffer();
    //CreateIndexBuffer();

    CompileShaders();

    glutDisplayFunc(RenderSceneCB);

    glutMainLoop();

    return 0;
}
