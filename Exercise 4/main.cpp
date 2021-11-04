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

#include <iostream>  // for printing user instructions and tests

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

#define NUMBER_MESHES 3  // 3 meshes: table, icosahedron and Utah teapot

GLuint VAO; // VAO = Vertex Array Object
GLuint VBO[NUMBER_MESHES]; // VBO = Vertex Buffer Object
GLuint IBO[NUMBER_MESHES]; // IBO = Index Buffer Object
GLuint gWVPLocation;

Scene scene;

void debug_print_VAO() {
    cout << "*** DEBUG PRINT VAO *** " << endl;
    cout << "VAO = " << VAO << endl;
    cout << endl;
}

void debug_print_VBO() {
    cout << "*** DEBUG PRINT VBO *** " << endl;
    for (int i = 0; i < NUMBER_MESHES; i++) {
        cout << "VBO[" << i << "] = " << VBO[i] << endl;
    }
    cout << endl;
}

void debug_print_IBO() {
    cout << "*** DEBUG PRINT IBO *** " << endl;
    for (int i = 0; i < NUMBER_MESHES; i++) {
        cout << "IBO[" << i << "] = " << IBO[i] << endl;
    }
    cout << endl;
}

void debug_print_versions() {
    // adapted from https://www.badprog.com/c-opengl-checking-the-freeglut-glew-and-opengl-version
    // Print OpenGL, FreeGLUT and GLEW versions
    string freeGlutVersion = to_string(glutGet(GLUT_VERSION));
    freeGlutVersion.insert(1, "."); // transforms 30000 into 3.0000)
    freeGlutVersion.insert(4, "."); // transforms 3.0000 into 3.00.00

    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
    cout << "FreeGLUT version: " << freeGlutVersion << endl;
    cout << "GLEW version: " <<
        GLEW_VERSION << "." << GLEW_VERSION_MAJOR << "." <<
        GLEW_VERSION_MINOR << "." << GLEW_VERSION_MICRO << endl << endl;
}

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    scene.computeArcball();

    scene.drawMesh(0, &VBO[0], &IBO[0], &gWVPLocation);  // draw table
    scene.drawMesh(1, &VBO[1], &IBO[1], &gWVPLocation);  // draw icosahedron
    scene.drawMesh(2, &VBO[2], &IBO[2], &gWVPLocation);  // draw Utah teapot

    glutPostRedisplay();

    glutSwapBuffers();
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

const char* pVSFileName = "vertex.vert";
const char* pFSFileName = "fragment.frag";

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


void KeyboardCB(unsigned char key, int x, int y)
{
    if (key == 27)  // ESC key
        exit(0);

    switch (key) {
    case GLUT_KEY_RIGHT:
    {
        scene.moveCameraRight(0.05);
        break;
    }
    case GLUT_KEY_LEFT:
    {
        scene.moveCameraLeft(0.05);
        break;
    }
    case GLUT_KEY_UP:
    {
        scene.moveCameraUp(0.05);
        break;
    }
    case GLUT_KEY_DOWN:
    {
        scene.moveCameraDown(0.05);
        break;
    }
    case GLUT_KEY_PAGE_UP:
    {
        scene.moveCameraFront(0.05);
        break;
    }
    case GLUT_KEY_PAGE_DOWN:
    {
        scene.moveCameraBack(0.05);
        break;
    }
    }
}

void SpecialKeyboardCB(int key, int x, int y)
{
    KeyboardCB(key, x, y);

}

// Adapted from https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
void MouseCB(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        scene.arcball_on = true;
        scene.mousePosX = scene.lastMouseX = x;
        scene.mousePosY = scene.lastMouseY = y;
        glutSetCursor(GLUT_CURSOR_CYCLE);
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        scene.arcball_on = false;
        glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    }
}

// Adapted from https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
void MotionCB(int x, int y) {
    if (scene.arcball_on) {  // if left button is pressed
        scene.mousePosX = x;
        scene.mousePosY = y;
    }
}

void MenuCB(int option) {
    switch (option)
    {
    case 0:
    {
        scene.setPerspecive();
        break;
    }

    case 1:
    {
        scene.setOrthogonal();
        break;
    }
    }
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

    glutInitContextVersion(3, 3);// Major version and minor version
    glutInitContextProfile(GLUT_CORE_PROFILE);

    int win = glutCreateWindow("Exercise 4");

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.2f, Green = 0.2f, Blue = 0.2f, Alpha = 0.0f;  // dark gray background
    glClearColor(Red, Green, Blue, Alpha);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //                        FOV |    Width   |    Height    | zNear | zFar;
    PersProjInfo pers_info = { 45, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 10.0f };
    //                          right | left | bottom | top | z near | z far
    OrthoProjInfo ortho_info = { 2.0f, -2.0f, -2.0f,   2.0f,   1.0f,  10.0f };
    //              camera position | perspective infos | orthographic infos | is_perspective
    scene = Scene({ 0.0f, 0.0f, -3.0f }, pers_info, ortho_info, true);

    //                             position        lenght height width 
    Mesh table = createTable({ 0.0f, 0.0f, 0.0f },  2.0f,  0.5f, 1.0f,
        // tabletop_thickness tableleg_length tableleg_width       color
                  0.1,              0.05,          0.2,      color_saddle_brown);
    scene.pushMesh(table);

    table.genVBO(&VBO[0]);
    table.genIBO(&IBO[0]);

    // icosahedron subdivided by 4, as required by the exercise
    Mesh icosahedron = createSubdividedIcosahedron(4, color_yellow);
    Transformation t1 = Transformation();
    t1.setScale({ 0.3f, 0.3f, 0.3f });  // scale icosahedron down
    t1.setTranslation({ 0.5f, 0.85f, 0.0f }); // moves icosahedron to the top of the table
    icosahedron.pushTransformation(t1);
    scene.pushMesh(icosahedron);

    icosahedron.genVBO(&VBO[1]);
    icosahedron.genIBO(&IBO[1]);

    Mesh teapot = createUtahTeapot(10, color_blue);
    Transformation t = Transformation();
    t.setScale({ 0.15f, 0.15f, 0.15f });  // original teapot is too big! Resize it to be smaller
    t.setRotation({ -90.0f, 0.0f, 0.0f });  // rotates it to be in the correct upright position
    t.setTranslation({ -0.5f, 0.55, 0.0f });  // moves Utah teapot to the top of the table
    teapot.pushTransformation(t);
    scene.pushMesh(teapot);

    teapot.genVBO(&VBO[2]);
    teapot.genIBO(&IBO[2]);

    debug_print_versions();
    //debug_print_VAO();
    //debug_print_VBO();
    //debug_print_IBO();

    glutDisplayFunc(RenderSceneCB);
    glutKeyboardFunc(KeyboardCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutMouseFunc(MouseCB);
    glutMotionFunc(MotionCB);

    int projectionMenu = glutCreateMenu(MenuCB);
    glutSetMenu(projectionMenu);
    glutAddMenuEntry("Perspectiva", 0);
    glutAddMenuEntry("Ortogonal", 1);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    
    CompileShaders();

    glutMainLoop();

    glutDestroyWindow(win);
    return 0;
}
