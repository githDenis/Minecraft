#version 330 core

in vec2 TexCoord;

uniform sampler2D aTexture;

void main()
{
	vec4 color = texture(aTexture, TexCoord);

	if (color.r < 0.1 && color.g < 0.1 && color.b < 0.1)
	{
		discard;
	}
	gl_FragColor = color;
}