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

public:
	~UIMesh();
	void GenerateCrossTarget(int windowWidth, int windowHeight);
	void GenerateRectangle(float width, float height, int windowWidth, int windowHeight);
	void SetColor(const Color& color);
	void Init();

	unsigned int GetVAO() const;
	unsigned int GetVertexArraySize() const noexcept;
};