#version 330

// 0 = flat; 1 = Gouraud; 2 = Phong
uniform int shadingMode;
uniform vec3 gViewPos;

vec3 ka = vec3(0.3, 0.3, 0.3);
vec3 kd = vec3(0.6, 0.6, 0.6);
vec3 ks = vec3(1.0, 1.0, 1.0);
float specularStrength = 0.5;
uniform int shininess = 32;

in vec4 Color;
flat in vec4 flatColor;
uniform in vec3 viewPos;  // camera view position
in vec3 norm;
in vec3 FragPos;
in vec3 lightDir;
in vec3 lightCol;
//in vec3 lightPos;
in vec4 glPos;

out vec4 FragColor;

vec4 Phong()
{
    float lambertian = max(dot(lightDir, -norm), 0);
    vec4 ambient = vec4(ka, 1);
    vec4 diffuse = vec4(kd * lambertian * lightCol, 1);
    float spec = 0.0;
    vec3 viewDir = normalize(viewPos - vec3(FragPos));
    if(lambertian > 0.0) {
        vec3 vertPos = vec3(glPos);
        vec3 reflectDir = reflect(-viewDir, norm);      
        vec3 view = normalize(-vertPos); 

        float specAngle = max(dot(reflectDir, view), 0.0);
        spec = pow(specAngle, shininess);
    }
        
    vec4 specular = vec4(ks * specularStrength * spec * lightCol, 1.0);
    vec4 final = (ambient + diffuse + specular) * Color;
    return final;
}

void main()
{
    if(shadingMode == 0) {  // flat shading
        FragColor = flatColor;
    }
    else if(shadingMode == 1) {  // Gourard shading{
        FragColor = Color;
    }
    else {  // Phong shading
        FragColor = Phong();
    }
}
