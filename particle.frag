#version 410 core

in vec2 TexCoords;
in vec4 FragColor;

out vec4 color;

uniform sampler2D particleTexture;

void main()
{
    // Sample the texture. The texture gives the particle its shape and alpha (transparency).
    vec4 textureColor = texture(particleTexture, TexCoords);

    // The final color is the particle's color (passed from C++) modulated by the texture's alpha.
    // We use the texture's RED channel for alpha, which is a common technique for single-channel textures.
    // This makes the particle's shape defined by the texture, while its color tint is controlled from the C++ side.
    color = vec4(FragColor.rgb, FragColor.a * textureColor.a);

    // Discard fragments that are fully transparent to avoid rendering issues.
    if (color.a < 0.01) {
        discard;
    }
}