#version 410 core

in vec2 TexCoords;
in vec4 FragColor; // Contains the alpha for fading, passed from C++

out vec4 color;

uniform sampler2D particleTexture;

void main()
{
    // Sample the smoke texture to get its shape (alpha)
    vec4 texColor = texture(particleTexture, TexCoords);

    // Force the smoke color to be a semi-transparent gray
    // The final alpha is a product of the texture's alpha and the particle's lifetime alpha
    color = vec4(0.6, 0.6, 0.6, texColor.a * FragColor.a);

    // Discard fragments that are almost fully transparent to avoid rendering artifacts
    if (color.a < 0.01) {
        discard;
    }
}