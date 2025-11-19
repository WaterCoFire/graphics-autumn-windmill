#version 410 core

in vec3 fragNormal;
in vec3 fragPos;

out vec4 color;

uniform vec3 lightPos; // Controllable light
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 objectColor;
uniform float shininess;

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);

    // === Independent ambient light ===
    vec3 ambient = 0.8 * ambientColor;

    // === Controllable light - diffuse reflection ===
    float diff = max(dot(norm, lightDir), 0.0);
    // Enhance diffuse reflection effect by multiplying it by 1.2
    vec3 diffuse = 1.2 * diff * lightColor;

    // === Controllable light - specular reflection ===
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // Specular reflection effect
    vec3 specular = 1.0 * spec * lightColor;

    // Ambient light and diffuse light are "colored" by objectColor
    vec3 ambient_light  = ambient * objectColor;
    vec3 diffuse_light  = diffuse * objectColor;

    // Specular light directly uses the light source's color (green), making the highlight very prominent
    vec3 specular_light = specular;

    // Composition
    vec3 result = ambient_light + diffuse_light + specular_light;
    color = vec4(result, 1.0);
}