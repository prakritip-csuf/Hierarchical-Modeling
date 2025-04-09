#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 color;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;
uniform int useLighting;

in vec3 FragPos;
in vec3 Normal;
in vec3 FragColor;

out vec4 FragColorOutput;

void main() {
    if (useLighting == 0) { 
        FragColorOutput = vec4(FragColor, 1.0); // Use per-vertex color with no lighting
        return;
    }

    // Normalize normal
    vec3 norm = normalize(Normal);

    // Compute light direction
    vec3 lightDir = normalize(light.position - FragPos);

    // Ambient component
    vec3 ambient = light.ambient * material.color;

    // Diffuse component
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.color;

    // Specular component
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = light.specular * spec * 2.0;

    // Combine lighting components
    vec3 result = ambient + diffuse + specular;
    FragColorOutput = vec4(result, 1.0);
}

