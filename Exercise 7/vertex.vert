#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 Normal;

uniform mat4 gWVP;
// 0 = flat; 1 = Gouraud; 2 = Phong
uniform int shadingMode;

// ka = ambient; kd = diffuse; ks = specular
vec3 ka, kd, ks;

out vec4 Color;  // color output
flat out vec3 flatColor;  // flat = without interpolation

void main()
{
    gl_Position = gWVP * vec4(Position, 1.0);
    
    if(shadingMode == 0) {  // flat shading
    
    }
    Color = vec4(inColor, 1.0);

}
