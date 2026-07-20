#pragma once

#include "Framework.h"
#include "Structs.h"

class Texture
{
private:
	unsigned int texture;

public:
	unsigned int GetID() const noexcept
	{
		return texture;
	}

	Texture() noexcept {};
	explicit Texture(const char* filePath);
	~Texture();

	void Create();
	void SetImage(const char* filePath);
	UV GetUV(int imageID, int xDivider, int yDivider) noexcept;
};