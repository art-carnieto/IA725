#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 inColor;

uniform mat4 gWVP;

out vec4 Color;

void main()
{
    gl_Position = gWVP * vec4(Position, 1.0);
    Color = vec4(inColor, 1.0);
    //Color = vec4(0.49609375, 0.19921875, 0.0, 1.0); // brown color
    //Color = vec4(1.0, 0.84375, 0.0, 1.0); // yellow color
    //Color = vec4(0, 0, 1, 1.0); // blue color
}
