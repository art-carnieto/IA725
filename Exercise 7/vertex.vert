#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 Normal;

uniform mat4 gWVP;
// 0 = flat; 1 = Gouraud; 2 = Phong
uniform int shadingMode;
uniform mat4 gFragPos;
uniform vec3 gViewPos;

// ka = ambient; kd = diffuse; ks = specular
vec3 ka = vec3(0.3, 0.3, 0.3);
vec3 kd = vec3(0.6, 0.6, 0.6);
vec3 ks = vec3(1.0, 1.0, 1.0);
float specularStrength = 0.5;
uniform int shininess = 32;

uniform vec3 lightPosition = vec3(-2.0, 2.0, 0.5);
uniform vec3 lightDirection = vec3(1.0, -1.0, 0.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

out vec4 Color;  // color output
flat out vec4 flatColor;  // flat = without interpolation
out vec3 norm;
out vec3 FragPos;
out vec3 lightDir;
out vec3 lightCol;
out vec3 lightPos;
out vec3 viewPos;
out vec4 glPos;

void main()
{
    gl_Position = gWVP * vec4(Position, 1.0);
    norm = normalize(Normal);
    FragPos = vec3(gFragPos);
    viewPos = gViewPos;
    lightDir = lightDirection;
    lightCol = lightColor;
    lightPos = lightPosition;
    glPos = gWVP * vec4(Position, 1.0);

    if(shadingMode == 0 || shadingMode == 1) {  // flat or Gourard shading
        float lambertian = max(dot(lightDirection, -norm), 0);
        vec4 ambient = vec4(ka, 1);
        vec4 diffuse = vec4(kd * lambertian * lightColor, 1);
        float spec = 0.0;
        vec3 viewDir = normalize(lightPosition - vec3(gFragPos));
        if(lambertian > 0.0) {
            vec3 vertPos = vec3(gWVP * vec4(Position, 1.0));
            vec3 reflectDir = reflect(-viewDir, norm);      
            vec3 view = normalize(-vertPos); 

            float specAngle = max(dot(reflectDir, view), 0.0);
            spec = pow(specAngle, shininess);
        }
        
        vec4 specular = vec4(ks * specularStrength * spec * lightColor, 1.0); 
        if(shadingMode == 0) flatColor = (ambient + diffuse + specular) * vec4(inColor, 1.0);  // flat
        else Color = (ambient + diffuse + specular) * vec4(inColor, 1.0);  // Gourard
    }
    else {
        Color = vec4(inColor, 1.0);
    }

}
