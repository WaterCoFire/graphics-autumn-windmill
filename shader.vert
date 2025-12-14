#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 aTexCoords;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMat;

void main() {
    gl_Position = proj * view * model * vec4(position, 1.0);
    fragPos = vec3(model * vec4(position, 1.0));
    fragNormal = normalMat * normal;
    TexCoords = aTexCoords;
}