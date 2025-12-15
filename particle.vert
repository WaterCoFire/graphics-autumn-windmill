#version 410 core

// Per-vertex attribute (for the quad)
layout (location = 0) in vec3 aPos;

// Instanced attributes (one per particle)
layout (location = 1) in vec4 particlePosAndSize; // .xyz = position, .w = size
layout (location = 2) in vec4 particleColor;

// Uniforms
uniform mat4 view;
uniform mat4 projection;

// Outputs to fragment shader
out vec2 TexCoords;
out vec4 FragColor;

void main()
{
    // The billboard technique requires extracting camera axes from the view matrix
    vec3 cameraRight_worldspace = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp_worldspace = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 particleCenter_worldspace = particlePosAndSize.xyz;
    float particleSize = particlePosAndSize.w;

    // Calculate the vertex position for the billboarded quad
    vec3 vertexPosition_worldspace = particleCenter_worldspace
    + cameraRight_worldspace * aPos.x * particleSize
    + cameraUp_worldspace * aPos.y * particleSize;

    // Standard MVP transformation
    gl_Position = projection * view * vec4(vertexPosition_worldspace, 1.0);

    // Set texture coordinates for the quad
    TexCoords = aPos.xy + vec2(0.5, 0.5);
    // Pass the particle's color to the fragment shader
    FragColor = particleColor;
}