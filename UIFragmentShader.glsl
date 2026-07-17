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
        vec4 color = texture(aTexture, TexCoord);
        if (color.r < 0.1 && color.g < 0.1 && color.b < 0.1)
        {
            discard;
        }
        else
        {
            FragColor = color;
        }
    }
    else
    {
        FragColor = vec4(Color, 1.f);
    }
}