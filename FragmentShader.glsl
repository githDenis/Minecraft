#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D aTexture;

void main()
{
	vec4 color = texture(aTexture, TexCoord);

	if (color.r < 0.1 && color.g < 0.1 && color.b < 0.1)
	{
		discard;
	}
	color.a *= 0.4;
	FragColor = color;
}