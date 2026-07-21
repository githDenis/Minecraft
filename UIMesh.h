#pragma once

#include "Framework.h"
#include "Structs.h"

class UIMesh
{
private:
	unsigned int VAO, VBO, colorVBO;
	unsigned int vertexArraySize;
	Vector<float> vertices;
	Vector<float> colors;
	Vector<float> textCoords;
	bool useTexture = false;

public:
	unsigned int GetVAO() const
	{
		return VAO;
	}

	unsigned int GetVertexArraySize() const noexcept
	{
		return vertexArraySize;
	}

	bool IsUseTexture() const noexcept
	{
		return useTexture;
	}

	~UIMesh();
	void GenerateCrossTarget(int windowWidth, int windowHeight) noexcept;
	void GenerateRectangle(float width, float height, int windowWidth, int windowHeight) noexcept;
	void GenerateFrame(float width, float height, int windowWidth, int windowHeight) noexcept;
	void SetColor(const Color& color) noexcept;
	void SetRectabgleUV(const UV& uv) noexcept;
	void Init();
};