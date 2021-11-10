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

#define NUMBER_MESHES 5  // 3 meshes: table, icosahedron and Utah teapot
                         // + 2 additional meshes to test the far clipping plane

GLuint VAO; // VAO = Vertex Array Object
GLuint VBO[NUMBER_MESHES]; // VBO = Vertex Buffer Object
GLuint IBO[NUMBER_MESHES]; // IBO = Index Buffer Object
GLuint gWVPLocation;

Scene scene;
float maximum_frustum_limit = 30.0f;  // maximum limit for the far clipping plane
float minimum_frustum_limit = 0.1f;  // minimum limit for the near clipping plane
float interaction_intensity = 0.5f;  // defines how fast the camera and clipping 
                                     // planes moves on user interaction
bool is_wireframe = false;

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

void debug_print_clippingPlanes() {
    cout << "near Z = " << scene.getNearClippingPlane();
    cout << "  |  far Z = " << scene.getFarClippingPlane() << endl;
}

void debug_print_cameraPosition(string move_direction) {
    Vector3f camPos = scene.getCamera().getCameraPos();
    cout << "Camera moved " << move_direction << "  |  Camera position =  { ";
    cout << camPos[0] << "  " << camPos[1] << "  " << camPos[2] << "}" << endl;
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
    scene.drawMesh(3, &VBO[3], &IBO[3], &gWVPLocation);  // draw cylinder
    scene.drawMesh(4, &VBO[4], &IBO[4], &gWVPLocation);  // draw cone

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
    switch (key) {
    case 27:  // ESC key
    {
        cout << "Goodbye!" << endl;
        exit(0);
    }
    case '+':
    {
        if (interaction_intensity < 5.0f)
            interaction_intensity += 0.1;
        cout << "Interaction intensity changed to " << interaction_intensity << endl;
        break;
    }
    case '-':
    {
        if (interaction_intensity - 0.1f > 0.1f)
            interaction_intensity -= 0.1;
        cout << "Interaction intensity changed to " << interaction_intensity << endl;
        break;
    }
    
    case 'u':
    {
        float currentNearZ = scene.getNearClippingPlane();
        if(currentNearZ + interaction_intensity < scene.getFarClippingPlane())  // never passes far planning clip
            scene.setNearClippingPlane(currentNearZ + interaction_intensity);
        debug_print_clippingPlanes();
        break;
    }
    case 'j':
    {
        float currentNearZ = scene.getNearClippingPlane();
        if (currentNearZ - interaction_intensity > minimum_frustum_limit)  // sets 0.1 as the minimum for near Z
            scene.setNearClippingPlane(currentNearZ - interaction_intensity);
        debug_print_clippingPlanes();
        break;
    }
    case 'i':
    {
        float currentFarZ = scene.getFarClippingPlane();
        if (currentFarZ + interaction_intensity < maximum_frustum_limit)  // never passes the far clipping plane limit (pre-defined)
            scene.setFarClippingPlane(currentFarZ + interaction_intensity);
        debug_print_clippingPlanes();
        break;
    }
    case 'k':
    {
        float currentFarZ = scene.getFarClippingPlane();
        if (currentFarZ - interaction_intensity > scene.getNearClippingPlane())  // never passes the near clipping plane
            scene.setFarClippingPlane(currentFarZ - interaction_intensity);
        debug_print_clippingPlanes();
        break;
    }
    case 'z':
    {
        if (!is_wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // wireframe mode
            is_wireframe = true;
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // fill mode
            is_wireframe = false;
        }
    }
    }
}

void SpecialKeyboardCB(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_RIGHT:
    {
        scene.moveCameraRight(interaction_intensity);
        debug_print_cameraPosition("right");
        break;
    }
    case GLUT_KEY_LEFT:
    {
        scene.moveCameraLeft(interaction_intensity);
        debug_print_cameraPosition("left");
        break;
    }
    case GLUT_KEY_UP:
    {
        scene.moveCameraUp(interaction_intensity);
        debug_print_cameraPosition("up");
        break;
    }
    case GLUT_KEY_DOWN:
    {
        scene.moveCameraDown(interaction_intensity);
        debug_print_cameraPosition("down");
        break;
    }
    case GLUT_KEY_PAGE_UP:
    {
        scene.moveCameraFront(interaction_intensity);
        debug_print_cameraPosition("forward");
        break;
    }
    case GLUT_KEY_PAGE_DOWN:
    {
        scene.moveCameraBack(interaction_intensity);
        debug_print_cameraPosition("backward");
        break;
    }
    }
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

    int win = glutCreateWindow("Exercise 6");

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

    // additional meshes to test the clipping planes
    Mesh cylinder = createCylinder(10, 0.2f, 2.0f, color_red);
    Transformation t_cylinder;
    t_cylinder.setTranslation({ -0.5f, 0.0f, 29.0f });
    cylinder.pushTransformation(t_cylinder);
    scene.pushMesh(cylinder);

    cylinder.genVBO(&VBO[3]);
    cylinder.genIBO(&IBO[3]);

    Mesh cone = createCone(20, 0.5f, 1.5f, color_lime);
    Transformation t_cone;
    t_cone.setTranslation({ 0.5f, 0.0f, 20.0f });
    cone.pushTransformation(t_cone);
    scene.pushMesh(cone);

    cone.genVBO(&VBO[4]);
    cone.genIBO(&IBO[4]);

    debug_print_versions();
    //debug_print_VAO();
    //debug_print_VBO();
    //debug_print_IBO();

    cout << "RGB to HSV tests:" << endl;
    cout << "redRGB = " << color_red[0] << "  " << color_red[1] << "  " << color_red[2] << endl;
    Vector3f red = RGBtoHSV(color_red[0], color_red[1], color_red[2]);
    cout << "redHSV = " << red[0] << "  " << red[1] << "  " << red[2] << endl << endl;

    cout << "limeRGB = " << color_lime[0] << "  " << color_lime[1] << "  " << color_lime[2] << endl;
    Vector3f lime = RGBtoHSV(color_lime[0], color_lime[1], color_lime[2]);
    cout << "limeHSV = " << lime[0] << "  " << lime[1] << "  " << lime[2] << endl << endl;

    cout << "blueRGB = " << color_blue[0] << "  " << color_blue[1] << "  " << color_blue[2] << endl;
    Vector3f blue = RGBtoHSV(color_blue[0], color_blue[1], color_blue[2]);
    cout << "blueHSV = " << blue[0] << "  " << blue[1] << "  " << blue[2] << endl << endl;

    cout << "blackRGB = " << color_black[0] << "  " << color_black[1] << "  " << color_black[2] << endl;
    Vector3f black = RGBtoHSV(color_black[0], color_black[1], color_black[2]);
    cout << "blackHSV = " << black[0] << "  " << black[1] << "  " << black[2] << endl << endl;

    cout << "whiteRGB = " << color_white[0] << "  " << color_white[1] << "  " << color_white[2] << endl;
    Vector3f white = RGBtoHSV(color_white[0], color_white[1], color_white[2]);
    cout << "whiteHSV = " << white[0] << "  " << white[1] << "  " << white[2] << endl << endl;

    cout << "yellowRGB = " << color_yellow[0] << "  " << color_yellow[1] << "  " << color_yellow[2] << endl;
    Vector3f yellow = RGBtoHSV(color_yellow[0], color_yellow[1], color_yellow[2]);
    cout << "yellowHSV = " << yellow[0] << "  " << yellow[1] << "  " << yellow[2] << endl << endl;

    cout << "cyanRGB = " << color_cyan[0] << "  " << color_cyan[1] << "  " << color_cyan[2] << endl;
    Vector3f cyan = RGBtoHSV(color_cyan[0], color_cyan[1], color_cyan[2]);
    cout << "cyanHSV = " << cyan[0] << "  " << cyan[1] << "  " << cyan[2] << endl << endl;

    cout << "magentaRGB = " << color_magenta[0] << "  " << color_magenta[1] << "  " << color_magenta[2] << endl;
    Vector3f magenta = RGBtoHSV(color_magenta[0], color_magenta[1], color_magenta[2]);
    cout << "magentaHSV = " << magenta[0] << "  " << magenta[1] << "  " << magenta[2] << endl << endl;

    cout << "saddle_brownRGB = " << color_saddle_brown[0] << "  " << color_saddle_brown[1] << "  " << color_saddle_brown[2] << endl;
    Vector3f saddle_brown = RGBtoHSV(color_saddle_brown[0], color_saddle_brown[1], color_saddle_brown[2]);
    cout << "saddle_brownHSV = " << saddle_brown[0] << "  " << saddle_brown[1] << "  " << saddle_brown[2] << endl << endl;

    cout << endl << "=====================================" << endl << endl;

    cout << "HSV to RGB tests:" << endl;
    Vector3f red_rgb = HSVtoRGB(0.0f, 1.0f, 1.0f);
    cout << "red_rgb = " << red_rgb[0] << "  " << red_rgb[1] << "  " << red_rgb[2] << endl;
    Vector3f red_rgb255 = RGBnormalizedFloatToRGBint(red_rgb[0], red_rgb[1], red_rgb[2]);
    cout << "red_rgb255 = " << red_rgb255[0] << "  " << red_rgb255[1] << "  " << red_rgb255[2] << endl << endl;

    Vector3f lime_rgb = HSVtoRGB(120.0f, 1.0f, 1.0f);
    cout << "lime_rgb = " << lime_rgb[0] << "  " << lime_rgb[1] << "  " << lime_rgb[2] << endl;
    Vector3f lime_rgb255 = RGBnormalizedFloatToRGBint(lime_rgb[0], lime_rgb[1], lime_rgb[2]);
    cout << "lime_rgb255 = " << lime_rgb255[0] << "  " << lime_rgb255[1] << "  " << lime_rgb255[2] << endl << endl;

    Vector3f blue_rgb = HSVtoRGB(240.0f, 1.0f, 1.0f);
    cout << "blue_rgb = " << blue_rgb[0] << "  " << blue_rgb[1] << "  " << blue_rgb[2] << endl;
    Vector3f blue_rgb255 = RGBnormalizedFloatToRGBint(blue_rgb[0], blue_rgb[1], blue_rgb[2]);
    cout << "blue_rgb255 = " << blue_rgb255[0] << "  " << blue_rgb255[1] << "  " << blue_rgb255[2] << endl << endl;

    Vector3f black_rgb = HSVtoRGB(0.0f, 0.0f, 0.0f);
    cout << "black_rgb = " << black_rgb[0] << "  " << black_rgb[1] << "  " << black_rgb[2] << endl;
    Vector3f black_rgb255 = RGBnormalizedFloatToRGBint(black_rgb[0], black_rgb[1], black_rgb[2]);
    cout << "black_rgb255 = " << black_rgb255[0] << "  " << black_rgb255[1] << "  " << black_rgb255[2] << endl << endl;

    Vector3f white_rgb = HSVtoRGB(0.0f, 0.0f, 1.0f);
    cout << "white_rgb = " << white_rgb[0] << "  " << white_rgb[1] << "  " << white_rgb[2] << endl;
    Vector3f white_rgb255 = RGBnormalizedFloatToRGBint(white_rgb[0], white_rgb[1], white_rgb[2]);
    cout << "white_rgb255 = " << white_rgb255[0] << "  " << white_rgb255[1] << "  " << white_rgb255[2] << endl << endl;

    Vector3f yellow_rgb = HSVtoRGB(60.0f, 1.0f, 1.0f);
    cout << "yellow_rgb = " << yellow_rgb[0] << "  " << yellow_rgb[1] << "  " << yellow_rgb[2] << endl;
    Vector3f yellow_rgb255 = RGBnormalizedFloatToRGBint(yellow_rgb[0], yellow_rgb[1], yellow_rgb[2]);
    cout << "yellow_rgb255 = " << yellow_rgb255[0] << "  " << yellow_rgb255[1] << "  " << yellow_rgb255[2] << endl << endl;

    Vector3f cyan_rgb = HSVtoRGB(180.0f, 1.0f, 1.0f);
    cout << "cyan_rgb = " << cyan_rgb[0] << "  " << cyan_rgb[1] << "  " << cyan_rgb[2] << endl;
    Vector3f cyan_rgb255 = RGBnormalizedFloatToRGBint(cyan_rgb[0], cyan_rgb[1], cyan_rgb[2]);
    cout << "cyan_rgb255 = " << cyan_rgb255[0] << "  " << cyan_rgb255[1] << "  " << cyan_rgb255[2] << endl << endl;

    Vector3f magenta_rgb = HSVtoRGB(300.0f, 1.0f, 1.0f);
    cout << "magenta_rgb = " << magenta_rgb[0] << "  " << magenta_rgb[1] << "  " << magenta_rgb[2] << endl;
    Vector3f magenta_rgb255 = RGBnormalizedFloatToRGBint(magenta_rgb[0], magenta_rgb[1], magenta_rgb[2]);
    cout << "magenta_rgb255 = " << magenta_rgb255[0] << "  " << magenta_rgb255[1] << "  " << magenta_rgb255[2] << endl << endl;
    
    int saddle_brown_original[] = { 139,69,19 };
    Vector3f saddle_brown_norm = RGBintToRGBnormalizedFloat(saddle_brown_original[0], saddle_brown_original[1], saddle_brown_original[2]);
    Vector3f saddle_brown_HSV = RGBtoHSV(saddle_brown_norm[0], saddle_brown_norm[1], saddle_brown_norm[2]);
    Vector3f saddle_brown_RGB = HSVtoRGB(saddle_brown_HSV[0], saddle_brown_HSV[1], saddle_brown_HSV[2]);
    Vector3f saddle_brown_RGB255 = RGBnormalizedFloatToRGBint(saddle_brown_RGB[0], saddle_brown_RGB[1], saddle_brown_RGB[2]);

    cout << "saddle_brown_original = " << saddle_brown_original[0] << "  " << saddle_brown_original[1] << "  " << saddle_brown_original[2] << endl;
    cout << "saddle_brown_norm = " << saddle_brown_norm[0] << "  " << saddle_brown_norm[1] << "  " << saddle_brown_norm[2] << endl;
    cout << "saddle_brown_HSV = " << saddle_brown_HSV[0] << "  " << saddle_brown_HSV[1] << "  " << saddle_brown_HSV[2] << endl;
    cout << "saddle_brown_RGB = " << saddle_brown_RGB[0] << "  " << saddle_brown_RGB[1] << "  " << saddle_brown_RGB[2] << endl;
    cout << "saddle_brown_RGB255 = " << saddle_brown_RGB255[0] << "  " << saddle_brown_RGB255[1] << "  " << saddle_brown_RGB255[2] << endl << endl;

    int corn_flower_blue_original[] = { 100,149,237 };
    Vector3f corn_flower_blue_norm = RGBintToRGBnormalizedFloat(corn_flower_blue_original[0], corn_flower_blue_original[1], corn_flower_blue_original[2]);
    Vector3f corn_flower_blue_HSV = RGBtoHSV(corn_flower_blue_norm[0], corn_flower_blue_norm[1], corn_flower_blue_norm[2]);
    Vector3f corn_flower_blue_RGB = HSVtoRGB(corn_flower_blue_HSV[0], corn_flower_blue_HSV[1], corn_flower_blue_HSV[2]);
    Vector3f corn_flower_blue_RGB255 = RGBnormalizedFloatToRGBint(corn_flower_blue_RGB[0], corn_flower_blue_RGB[1], corn_flower_blue_RGB[2]);

    cout << "corn_flower_blue_original = " << corn_flower_blue_original[0] << "  " << corn_flower_blue_original[1] << "  " << corn_flower_blue_original[2] << endl;
    cout << "corn_flower_blue_norm = " << corn_flower_blue_norm[0] << "  " << corn_flower_blue_norm[1] << "  " << corn_flower_blue_norm[2] << endl;
    cout << "corn_flower_blue_HSV = " << corn_flower_blue_HSV[0] << "  " << corn_flower_blue_HSV[1] << "  " << corn_flower_blue_HSV[2] << endl;
    cout << "corn_flower_blue_RGB = " << corn_flower_blue_RGB[0] << "  " << corn_flower_blue_RGB[1] << "  " << corn_flower_blue_RGB[2] << endl;
    cout << "corn_flower_blue_RGB255 = " << corn_flower_blue_RGB255[0] << "  " << corn_flower_blue_RGB255[1] << "  " << corn_flower_blue_RGB255[2] << endl << endl;

    int dark_slate_gray_original[] = { 47, 79, 79 };
    Vector3f dark_slate_gray_norm = RGBintToRGBnormalizedFloat(dark_slate_gray_original[0], dark_slate_gray_original[1], dark_slate_gray_original[2]);
    Vector3f dark_slate_gray_HSV = RGBtoHSV(dark_slate_gray_norm[0], dark_slate_gray_norm[1], dark_slate_gray_norm[2]);
    Vector3f dark_slate_gray_RGB = HSVtoRGB(dark_slate_gray_HSV[0], dark_slate_gray_HSV[1], dark_slate_gray_HSV[2]);
    Vector3f dark_slate_gray_RGB255 = RGBnormalizedFloatToRGBint(dark_slate_gray_RGB[0], dark_slate_gray_RGB[1], dark_slate_gray_RGB[2]);

    cout << "dark_slate_gray_original = " << dark_slate_gray_original[0] << "  " << dark_slate_gray_original[1] << "  " << dark_slate_gray_original[2] << endl;
    cout << "dark_slate_gray_norm = " << dark_slate_gray_norm[0] << "  " << dark_slate_gray_norm[1] << "  " << dark_slate_gray_norm[2] << endl;
    cout << "dark_slate_gray_HSV = " << dark_slate_gray_HSV[0] << "  " << dark_slate_gray_HSV[1] << "  " << dark_slate_gray_HSV[2] << endl;
    cout << "dark_slate_gray_RGB = " << dark_slate_gray_RGB[0] << "  " << dark_slate_gray_RGB[1] << "  " << dark_slate_gray_RGB[2] << endl;
    cout << "dark_slate_gray_RGB255 = " << dark_slate_gray_RGB255[0] << "  " << dark_slate_gray_RGB255[1] << "  " << dark_slate_gray_RGB255[2] << endl << endl;

    int light_salmon_original[] = { 255,160,122 };
    Vector3f light_salmon_norm = RGBintToRGBnormalizedFloat(light_salmon_original[0], light_salmon_original[1], light_salmon_original[2]);
    Vector3f light_salmon_HSV = RGBtoHSV(light_salmon_norm[0], light_salmon_norm[1], light_salmon_norm[2]);
    Vector3f light_salmon_RGB = HSVtoRGB(light_salmon_HSV[0], light_salmon_HSV[1], light_salmon_HSV[2]);
    Vector3f light_salmon_RGB255 = RGBnormalizedFloatToRGBint(light_salmon_RGB[0], light_salmon_RGB[1], light_salmon_RGB[2]);

    cout << "light_salmon_original = " << light_salmon_original[0] << "  " << light_salmon_original[1] << "  " << light_salmon_original[2] << endl;
    cout << "light_salmon_norm = " << light_salmon_norm[0] << "  " << light_salmon_norm[1] << "  " << light_salmon_norm[2] << endl;
    cout << "light_salmon_HSV = " << light_salmon_HSV[0] << "  " << light_salmon_HSV[1] << "  " << light_salmon_HSV[2] << endl;
    cout << "light_salmon_RGB = " << light_salmon_RGB[0] << "  " << light_salmon_RGB[1] << "  " << light_salmon_RGB[2] << endl;
    cout << "light_salmon_RGB255 = " << light_salmon_RGB255[0] << "  " << light_salmon_RGB255[1] << "  " << light_salmon_RGB255[2] << endl << endl;

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
