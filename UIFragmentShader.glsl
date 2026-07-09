#version 330 core

in vec3 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D aTexture;
uniform bool useTexture;

void main()
{
    if (useTexture)
    {
        FragColor = texture(aTexture, TexCoord);
    }
    else
    {
        FragColor = vec4(Color, 1.f);
    }
}