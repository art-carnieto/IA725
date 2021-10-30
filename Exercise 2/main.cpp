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

#include <iostream>  // for user interaction on prompt

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

//Ting: VBO contem um amontoado de vertices. Em vertex shader nao se distinguem as primitiva.
//Sao os programadores que separam os vertices em grupos diferentes para facilitar o processamento
// por objeto. Neste projeto vale a pena distinguir os vertices em 3 conjuntos (mesa, esfera e bule).
#define NUMBER_MESHES 5  // 5 because of the table drawing: it uses 5 transformed cubes!

//Ting: eh recomendavel criar um VAO
GLuint VAO; // VAO = Vertex Array Object
GLuint VBO[NUMBER_MESHES]; // VBO = Vertex Buffer Object
GLuint IBO[NUMBER_MESHES]; // IBO = Index Buffer Object
GLuint gWVPLocation;

Scene scene;

static GLuint first = 0;

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

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (first == 1) {
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

    }

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
    tabletop.pushTransformation(t_tabletop);
    scene.pushMesh(tabletop);

    // table leg 1: left back
    Mesh table_leg_1 = createCube(color);
    Transformation t_table_leg_1 = Transformation();
    t_table_leg_1.setTranslation({ (-table_length / 2) + (tableleg_length / 2) + position[0], 0.0f + position[1], (-table_width / 2) + (tableleg_width / 2) + position[2] });
    t_table_leg_1.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    table_leg_1.pushTransformation(t_table_leg_1);
    scene.pushMesh(table_leg_1);


    // table leg 2: left front
    Mesh table_leg_2 = createCube(color);
    Transformation t_table_leg_2 = Transformation();
    t_table_leg_2.setTranslation({ (-table_length / 2) + (tableleg_length / 2) + position[0], 0.0f + position[1], (+table_width / 2) - (tableleg_width / 2) + position[2] });
    t_table_leg_2.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    table_leg_2.pushTransformation(t_table_leg_2);
    scene.pushMesh(table_leg_2);

    // table leg 3: right back
    Mesh table_leg_3 = createCube(color);
    Transformation t_table_leg_3 = Transformation();
    t_table_leg_3.setTranslation({ (+table_length / 2) - (tableleg_length / 2) + position[0], 0.0f + position[1], (-table_width / 2) + (tableleg_width / 2) + position[2] });
    t_table_leg_3.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    table_leg_3.pushTransformation(t_table_leg_3);
    scene.pushMesh(table_leg_3);

    // table leg 4: right front
    Mesh table_leg_4 = createCube(color);
    Transformation t_table_leg_4 = Transformation();
    t_table_leg_4.setTranslation({ (+table_length / 2) - (tableleg_length / 2) + position[0], 0.0f + position[1], (+table_width / 2) - (tableleg_width / 2) + position[2] });
    t_table_leg_4.setScale({ tableleg_length, (table_height * 2) - (tabletop_thickness), tableleg_width });
    table_leg_4.pushTransformation(t_table_leg_4);
    scene.pushMesh(table_leg_4);
}

/*!
 *
 */
void Keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);

    if (first == 1) return;

    switch (key)
    {
    case '1':
        //Ting: Veja que a mesa eh associada a UM nome de VBO!
    {
        //                position       lenght height width tabletop_thickness tableleg_length tableleg_width       color
        createTable({ 0.0f, 0.0f, 0.0f }, 2.0f,  0.5f,  1.0f,      0.1,              0.05,          0.2,      color_saddle_brown);
    }

    //Ting: Veja que na funcao Mesh::genVBO(GLuint* VBO) eh gerado um nome de VBO!
    scene.genAllVBOs(&VBO[0]);
    scene.genAllIBOs(&IBO[0]);

    debug_print_VAO();
    debug_print_VBO();
    debug_print_IBO();

    first = 1;
    break;
    case '2':
    {
        Mesh icosahedron = createSubdividedIcosahedron(3, color_yellow);
        Transformation t1 = Transformation();
        t1.setScale({ 0.5f, 0.5f, 0.5f });  // scale it down to half so it's better to see it on screen
        icosahedron.pushTransformation(t1);
        scene.pushMesh(icosahedron);
    }
    //Ting: Ao inves de sobreescrever o nome anterior, o que acha de guardar numa outra posicao da memoria o nome do segundo
    //objeto para voce poder ter acesso aos dados dos dois objetos?
    scene.genAllVBOs(&VBO[0]);
    scene.genAllIBOs(&IBO[0]);

    debug_print_VAO();
    debug_print_VBO();
    debug_print_IBO();

    first = 1;
    break;
    case '3':
    {
        Mesh teapot = createUtahTeapot(10, color_blue);
        Transformation t = Transformation();
        t.setScale({ 0.4, 0.4, 0.4 });  // original teapot is too big! Resize it to be smaller
        t.setRotation({ -90.0f, 0.0f, 0.0f });  // rotates it to be in the correct upright position
        t.setTranslation({ 0.0f, -0.7, 0.0f });  // moves the object down to recenter it because the rotation moves it up
        teapot.pushTransformation(t);
        scene.pushMesh(teapot);
    }
    //Ting: E aqui nao poderia ser um terceiro nome numa terceira posicao? 
    scene.genAllVBOs(&VBO[0]);
    scene.genAllIBOs(&IBO[0]);

    debug_print_VAO();
    debug_print_VBO();
    debug_print_IBO();

    first = 1;
    break;
    }
    // Rerenderizar
    glutPostRedisplay();

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

    int win = glutCreateWindow("Exercise 2");

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;  // black background
    glClearColor(Red, Green, Blue, Alpha);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode

    //Ting: eh uma boa pratica criar um vertex array object para "conter" todos os estados de vertices
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //                        FOV |    Width   |    Height    | zNear | zFar;
    PersProjInfo pers_info = { 45, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 10.0f };
    //                          right | left | bottom | top | z near | z far
    OrthoProjInfo ortho_info = { 2.0f, -2.0f, -2.0f,   2.0f,   1.0f,  10.0f };
    //              camera position | perspective infos | orthographic infos | is_perspective
    scene = Scene({ 0.0f, 0.0f, -3.0f },  pers_info,         ortho_info,           true);

    bool valid_choice = false;
    char choice;  // User input to choose which object will be drawn

    //Ting: Num sistema interativo, o fluxo de controle eh orientado a eventos
    //Precisa-se registrar "calbacks"/rotinas de tratamento de eventos.
    //Veja no link https://www.opengl.org/resources/libraries/glut/spec3/node45.html
    //as funcoes de registro de rotinas para tratamento de eventos especificos
    glutDisplayFunc(RenderSceneCB);  //Ting: Etay
    glutKeyboardFunc(Keyboard);

    //Ting: Analise com cuidado a estrutura de dados dos codigos de Etay. Fiz pequenas alteracoes
    // so para podermos ter a tela inicializada com fundo preto e permitir que sejam teclado '1', '2', e '3'
    // sobre a janela de desenho para que o objeto selecionado seja mostrado.

    // while (!valid_choice) {
    //     cout << "Please type the option of which object to be drawn:" << endl;
    //     cout << "  1) Table" << endl;
    //     cout << "  2) Subdivided icosahedron (sphere)" << endl;
    //     cout << "  3) Utah teapot" << endl;
    //     cin >> choice;
    //     if (choice != '1' && choice != '2' && choice != '3')
    //         cout << "Option not recognized, please try again!" << endl << endl;
    //     else
    //         valid_choice = true;
    // }
    // if (choice == '1') {
    //     //                position       lenght height width tabletop_thickness tableleg_length tableleg_width       color
    //     createTable({ 0.0f, 0.0f, 0.0f }, 2.0f,  0.5f,  1.0f,      0.1,              0.05,          0.2,      color_saddle_brown);
    // }
    // else if (choice == '2') {
    //     //                                subdivision number  color
    //     Mesh icosahedron = createSubdividedIcosahedron(3, color_yellow);
    //     Transformation t1 = Transformation();
    //     t1.setScale({ 0.5f, 0.5f, 0.5f });  // scale it down to half so it's better to see it on screen
    //     icosahedron.pushTransformation(t1);
    //     scene.pushMesh(icosahedron);
    // }
    // else if (choice == '3') {
    //     //                subdivision number  color
    //     Mesh teapot = createUtahTeapot(10, color_blue);
    //     Transformation t = Transformation();
    //     t.setScale({ 0.4, 0.4, 0.4 });  // original teapot is too big! Resize it to be smaller
    //     t.setRotation({ -90.0f, 0.0f, 0.0f });  // rotates it to be in the correct upright position
    //     t.setTranslation({ 0.0f, -0.7, 0.0f });  // moves the object down to recenter it because the rotation moves it up
    //     teapot.pushTransformation(t);
    //     scene.pushMesh(teapot);
    // }


    // scene.genAllVBOs(&VBO[0]);
    // scene.genAllIBOs(&IBO[0]);

    CompileShaders();

    {
        cout << "Please type the option of which object to be drawn on the display area:" << endl;
        cout << "  1) Table" << endl;
        cout << "  2) Subdivided icosahedron (sphere)" << endl;
        cout << "  3) Utah teapot" << endl;
    }

    glutMainLoop();

    
    return 0;
}
