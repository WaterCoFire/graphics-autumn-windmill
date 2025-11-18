#version 410 core

in vec3 fragNormal;
in vec3 fragPos;

out vec4 color;

uniform vec3 lightPos; // Controllable green light
uniform vec3 viewPos;
uniform vec3 lightColor; // Green light
uniform vec3 ambientColor; // Warm yellow ambient (global) light
uniform vec3 objectColor;
uniform float shininess;

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);

    // === Independent ambient light ===
    vec3 ambient = 2 * ambientColor;

    // === Controllable light - diffuse reflection ===
    float diff = max(dot(norm, lightDir), 0.0);
    // Enhance diffuse reflection effect by multiplying it by 1.2
    vec3 diffuse = 1.2 * diff * lightColor;

    // === Controllable light - specular reflection ===
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // specular reflection effect
    vec3 specular = 1.0 * spec * lightColor;

    // === Final Composition ===
    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0);
}