#include "UIMesh.h"

UIMesh::~UIMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void UIMesh::GenerateCrossTarget(int windowWidth, int windowHeight)
{
	float aspect1 = (float)windowWidth / (float)windowHeight;

	float vertecies[] =
	{
		 0.f / aspect1,  0.03f,
		 0.f / aspect1, -0.03f,

		 0.03f / aspect1, 0.f,
		-0.03f / aspect1, 0.f,
	};

	vertexArraySize = sizeof(vertecies) / sizeof(float);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int UIMesh::GetVAO() const
{
	return VAO;
}

unsigned int UIMesh::GetVertexArraySize() const noexcept
{
	return vertexArraySize;
}