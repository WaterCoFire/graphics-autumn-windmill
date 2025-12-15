#version 410 core

in vec3 fragNormal;
in vec3 fragPos;
in vec2 TexCoords;

out vec4 color;

uniform vec3 lightPos; // Controllable light
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 objectColor;
uniform bool useTexture; // Use texture color or object color
uniform float shininess;

uniform sampler2D texture_diffuse1;
uniform bool u_unlit; // A switch to disable lighting

void main() {
    vec3 baseColor;
    if(useTexture)
        baseColor = texture(texture_diffuse1, TexCoords).rgb; // Use texture color
    else
        baseColor = objectColor; // Use uniform color

    // If u_unlit is true, skip all lighting calculations
    if(u_unlit) {
        color = vec4(baseColor, 1.0);
        return;
    }

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);

    // Independent ambient light
    vec3 ambient = 1.0 * ambientColor;

    // Controllable light - diffuse reflection
    float diff = max(dot(norm, lightDir), 0.0);
    // Enhance diffuse reflection effect by multiplying it by 1.2
    vec3 diffuse = 1.2 * diff * lightColor;

    // Controllable light - specular reflection
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // Specular reflection effect
    vec3 specular = 1.0 * spec * lightColor;

    // Ambient light and diffuse light are "colored" by objectColor
    vec3 ambient_light  = ambient * baseColor;
    vec3 diffuse_light  = diffuse * baseColor;

    // Specular light directly uses the light source's color (green), making the highlight very prominent
    vec3 specular_light = specular;

    // Composition
    vec3 result = ambient_light + diffuse_light + specular_light;
    color = vec4(result, 1.0);
}