#pragma once

#include "Framework.h"

class UIMesh
{
private:
	unsigned int VAO, VBO;
	unsigned int vertexArraySize;

public:
	~UIMesh();
	void GenerateCrossTarget(int windowWidth, int windowHeight);

	unsigned int GetVAO() const;
	unsigned int GetVertexArraySize() const noexcept;
};