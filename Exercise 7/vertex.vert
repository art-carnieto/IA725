#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 Normal;

uniform mat4 gWVP;
// 0 = flat; 1 = Gouraud; 2 = Phong
uniform int shadingMode;

// ka = ambient; kd = diffuse; ks = specular
//vec3 ka, kd, ks;
vec3 ka = vec3(1.0, 1.0, 1.0);
uniform vec3 lightDirection = vec3(1.0, -1.0, 0.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

out vec4 Color;  // color output
flat out vec3 flatColor;  // flat = without interpolation

void main()
{
    gl_Position = gWVP * vec4(Position, 1.0);
    
    if(shadingMode == 0) {  // flat shading
        vec4 ambient = vec4(ka, 1);
        vec4 diffuse = vec4(max(dot(lightDirection, -Normal), 0) * lightColor, 1);
        Color = (ambient + diffuse) * vec4(inColor, 1.0);
    }
    else {
        Color = vec4(inColor, 1.0);
    }

}
