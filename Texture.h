#pragma once

#include "Framework.h"
#include "Structs.h"

class Texture
{
private:
	unsigned int texture;
	UV uv;

public:
	Texture() noexcept {};
	explicit Texture(const char* filePath);
	~Texture();

	void Create();
	void SetImage(const char* filePath);
	unsigned int GetID() const noexcept;
	UV GetUV(int imageID,int divider);
};