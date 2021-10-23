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
#define NUMBER_MESHES 2

//Ting: eh recomendavel criar um VAO
GLuint VAO; // VAO = Vertex Array Object
GLuint VBO[NUMBER_MESHES]; // VBO = Vertex Buffer Object
GLuint IBO[NUMBER_MESHES]; // IBO = Index Buffer Object
GLuint gWVPLocation;

Scene scene;

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float Scale = 0.0f;  // scale is used to rotate the world

#ifdef _WIN64
    Scale += 0.5f;
#else
    Scale += 0.02f;
#endif
    Transformation world_transformation = Transformation();
    world_transformation.setRotation({ 0.0f, Scale, 0.0f });
    scene.setWorldTransformation(world_transformation);

    scene.drawAllMeshes(&VBO[0], &IBO[0], &gWVPLocation);

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

void createTable(Vector3f position,
        float table_length, float table_height, float table_width,
        float tabletop_thickness, float tableleg_length, float tableleg_width,
        Vector3f color) {
    
    // tabletop
    Mesh tabletop = createCube(color);
    Transformation t_tabletop = Transformation();
    t_tabletop.setTranslation({ 0.0f + position[0], table_height + position[1], 0.0f + position[2] });
    t_tabletop.setScale({ table_length, tabletop_thickness, table_width });
    tabletop.setTransformation(t_tabletop);
    scene.pushMesh(tabletop);

    // table leg 1: left back
    Mesh table_leg_1 = createCube(color);
    Transformation t_table_leg_1 = Transformation();
    t_table_leg_1.setTranslation({ (-table_length / 2) + (tableleg_length / 2) + position[0], 0.0f + position[1], (-table_width / 2) + (tableleg_width / 2) + position[2] });
    t_table_leg_1.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    table_leg_1.setTransformation(t_table_leg_1);
    scene.pushMesh(table_leg_1);


    // table leg 2: left front
    Mesh table_leg_2 = createCube(color);
    Transformation t_table_leg_2 = Transformation();
    t_table_leg_2.setTranslation({ (-table_length / 2) + (tableleg_length / 2) + position[0], 0.0f + position[1], (+table_width / 2) - (tableleg_width / 2) + position[2] });
    t_table_leg_2.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    table_leg_2.setTransformation(t_table_leg_2);
    scene.pushMesh(table_leg_2);

    // table leg 3: right back
    Mesh table_leg_3 = createCube(color);
    Transformation t_table_leg_3 = Transformation();
    t_table_leg_3.setTranslation({ (+table_length / 2) - (tableleg_length / 2) + position[0], 0.0f + position[1], (-table_width / 2) + (tableleg_width / 2) + position[2] });
    t_table_leg_3.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    table_leg_3.setTransformation(t_table_leg_3);
    scene.pushMesh(table_leg_3);

    // table leg 4: right front
    Mesh table_leg_4 = createCube(color);
    Transformation t_table_leg_4 = Transformation();
    t_table_leg_4.setTranslation({ (+table_length / 2) - (tableleg_length / 2) + position[0], 0.0f + position[1], (+table_width / 2) - (tableleg_width / 2) + position[2] });
    t_table_leg_4.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    table_leg_4.setTransformation(t_table_leg_4);
    scene.pushMesh(table_leg_4);
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

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode

    //Ting: eh uma boa pratica criar um vertex array object para "conter" todos os estados de vertices
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    PersProjInfo pers_info = { 45, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 10.0f };
    OrthoProjInfo ortho_info = { 2.0f, -2.0f, -2.0f, 2.0f, 1.0f, 10.0f };

    scene = Scene({ 0.0f, 0.0f, -3.0f }, pers_info, ortho_info, true);

    /*
    Mesh m3 = createSubdividedIcosahedron(3, color_green);
    Transformation t3 = Transformation();
    t3.setTranslation({ 1.0f, 0.0f, 0.0f });
    t3.setRotation({ 0.0f, 0.0f, 0.0f });
    t3.setScale({ 0.5f, 0.1f, 0.3f });
    m3.setTransformation(t3);
    scene.pushMesh(m3);
    */

    //createTable({ 0.0f, 0.0f, -1.0f }, 2.0f, 0.5f, 1.0f, 0.1, 0.05, 0.2, color_blue);

    Mesh teapot = createUtahTeapot(10, color_red);
    Transformation t = Transformation();
    t.setScale({ 0.5, 0.5, 0.5 });
    t.setRotation({ -90.0f, 0.0f, 0.0f });
    t.setTranslation({ 0.0f, -0.7, 0.0f });
    teapot.setTransformation(t);
    scene.pushMesh(teapot);

    scene.genAllVBOs(&VBO[0]);
    scene.genAllIBOs(&IBO[0]);

    CompileShaders();

    glutDisplayFunc(RenderSceneCB);

    glutMainLoop();

    return 0;
}
