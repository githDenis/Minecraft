#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D aTexture;

void main()
{
    vec4 color = texture(aTexture, TexCoord);

    if (color.r < 0.15 && color.g < 0.15 && color.b < 0.15)
    {
        discard;
    }
    else
    {
        FragColor = color;
    }
}