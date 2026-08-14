#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D aTexture;

void main()
{
    vec4 color = texture(aTexture, TexCoord);

    FragColor = color;

    color.a *= 0.4;
    FragColor = color;
}