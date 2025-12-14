#version 410 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    // Remove translation from the view matrix so the skybox follows the camera
    mat4 viewNoTranslation = mat4(mat3(view));
    vec4 pos = projection * viewNoTranslation * vec4(aPos, 1.0);
    // Use the "z = w" trick to ensure the skybox is always at the far depth plane
    gl_Position = pos.xyww;
}
