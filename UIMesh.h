#pragma once

#include "Framework.h"
#include "Structs.h"

class UIMesh
{
private:
	unsigned int VAO, VBO, colorVBO;
	unsigned int vertexArraySize;
	Vector<float> vertecies;
	Vector<float> colors;
	Vector<float> textCoords;
	bool useTexture = false;

public:
	~UIMesh();
	void GenerateCrossTarget(int windowWidth, int windowHeight) noexcept;
	void GenerateRectangle(float width, float height, int windowWidth, int windowHeight) noexcept;
	void SetColor(const Color& color) noexcept;
	void SetRectabgleUV(const UV& uv) noexcept;
	void Init();

	unsigned int GetVAO() const;
	unsigned int GetVertexArraySize() const noexcept;
	bool IsUseTexture() const noexcept;
};