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

#include "Scene.hpp"
#include "Colors.hpp"

#include <iostream> // for tests using cout

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

//Ting: eh recomendavel criar um VAO
GLuint VAO; // VAO = Vertex Array Object

GLuint VBO; // VBO = Vertex Buffer Object
GLuint IBO; // IBO = Index Buffer Object
GLuint gWVPLocation;

vector<Vertex> Vertices;
vector<unsigned int> Indices;
int indices_offset = 0;

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float Scale = 0.0f;  // scale is used to rotate the world

#ifdef _WIN64
    Scale += 0.007f;
#else
    Scale += 0.02f;
#endif

    // rotation around Y axis
    Matrix4f Rotation(
        cosf(Scale), 0.0f, -sinf(Scale), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinf(Scale), 0.0f, cosf(Scale), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    // doesn't translate
    Matrix4f Translation(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    Matrix4f World = Translation * Rotation;

    // camera is positioned on X = -3
    Vector3f CameraPos(0.0f, 0.0f, -3.0f);
    Vector3f U(1.0f, 0.0f, 0.0f);   // U
    Vector3f V(0.0f, 1.0f, 0.0f);   // V
    Vector3f N(0.0f, 0.0f, 1.0f);   // W

    Matrix4f Camera(
        U.x, U.y, U.z, -CameraPos.x,
        V.x, V.y, V.z, -CameraPos.y,
        N.x, N.y, N.z, -CameraPos.z,
        0.0f, 0.0f, 0.0f, 1.0f);

    float VFOV = 45.0f; // FOV = Field of View
    float tanHalfVFOV = tanf(ToRadian(VFOV / 2.0f));    // AB
    float d = 1 / tanHalfVFOV;
    float ar = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;  // aspect ratio

    float NearZ = 1.0f;         // near plane distance
    float FarZ = 10.0f;         // far plane distance

    float zRange = NearZ - FarZ;    // Frustum

    float A = (-FarZ - NearZ) / zRange;
    float B = 2.0f * FarZ * NearZ / zRange;

    // Mpp = Matrix of Parallel Projection
    Matrix4f Projection(
        d / ar, 0.0f, 0.0f, 0.0f,
        0.0f, d, 0.0f, 0.0f,
        0.0f, 0.0f, A, B,
        0.0f, 0.0f, 1.0f, 0.0f);

    // World View Projection
    Matrix4f WVP = Projection * Camera * World;

    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, &WVP.m[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    glDrawElements(GL_TRIANGLES, (GLsizei)Indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glutPostRedisplay();

    glutSwapBuffers();
}

static void CreateVertexBuffer()
{
    if (Vertices.size() == 0) return;   // in case Vertices is empty
    //Ting: eh uma boa pratica criar um vertex array object para "conter" todos os estados de vertices
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    Vertex* Vertices_final = &Vertices[0];

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertices_final[0]), Vertices_final, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
    if (Indices.size() == 0) return;    // in case Indices is empty
    unsigned int* Indices_final = &Indices[0];

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(Indices_final[0]), Indices_final, GL_STATIC_DRAW);
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    int x = 200;
    int y = 100;
    glutInitWindowPosition(x, y);

    //Ting: setar profile de OpenGL explicitamente eh uma boa pratica ... 
    glutInitContextVersion(3, 3);// Major version and minor version
    glutInitContextProfile(GLUT_CORE_PROFILE);

    int win = glutCreateWindow("Mesa");

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode

    Mesh m = createSubdividedIcosahedron(3, color_red);

    Vertices = m.getVertices();
    Indices = m.getIndices();

    Transformation t;
    t.setTranslation({ 2.0f, 0.0f, 0.0f });
    t.setRotation({ 0.0f, 1.0f, 0.0f });
    t.setScale({ 1.0f, 1.0f, 2.0f });
    t.calcFinalTransformation();

    cout << "t = \n";
    t.getFinalTransformation().Print();
    cout << endl;

    /*
    cout << "number of triangles = " << m.getNumberTriangles() << endl;
    cout << "get first triangle = " << m.getFirstTriangleIndexPos() << endl;
    cout << "get last triangle = " << m.getLastTriangleIndexPos() << endl;

    m.debug_print_vertices();
    m.debug_print_indices();
    */

    CreateVertexBuffer();
    CreateIndexBuffer();

    CompileShaders();

    glutDisplayFunc(RenderSceneCB);

    glutMainLoop();

    return 0;
}
