#include "Texture.h"

Texture::Texture(const char* filePath)
{
	SetImage(filePath);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

void Texture::Create()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetImage(const char* filePath)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nChannels;
	unsigned char* imageBuf = stbi_load(filePath, &width, &height, &nChannels, NULL);

	if (imageBuf)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, nChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0,
			nChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, imageBuf);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(imageBuf);
}

unsigned int Texture::GetID() const noexcept
{
	return texture;
}

UV Texture::GetUV(int imageID, int divider)
{
	float imageSize = 1.f / divider;

	int x = imageID % divider;
	int y = imageID / divider;

	uv.u0 = x * imageSize;
	uv.v0 = y * imageSize;
	uv.u1 = uv.u0 + imageSize;
	uv.v1 = uv.v0 + imageSize;
	return uv;
}