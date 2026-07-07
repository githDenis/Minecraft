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

	static constexpr int vertexArraySize = sizeof(vertecies) / sizeof(float);
	this->vertexArraySize = vertexArraySize;
	this->vertecies.AddArray(vertecies, vertexArraySize);
}

void UIMesh::GenerateRectangle(float width, float height, int windowWidth, int windowHeight)
{
	float aspect1 = (float)windowWidth / (float)windowHeight;

	float x1 = width / -2;
	float x2 = width / 2;
	float y1 = height / -2;
	float y2 = height / 2;

	float vertecies[] =
	{
		x1 / aspect1, y1,
		x1 / aspect1, y2,
		x2 / aspect1, y2,

		x2 / aspect1, y2,
		x2 / aspect1, y1,
		x1 / aspect1, y1,
	};

	static constexpr int vertexArraySize = sizeof(vertecies) / sizeof(float);
	this->vertexArraySize = vertexArraySize;
	this->vertecies.AddArray(vertecies, vertexArraySize);
}

void UIMesh::SetColor(const Color& color)
{
	for (int i = 0; i < vertecies.GetSize(); i++)
	{
		colors.Add(color.red);
		colors.Add(color.green);
		colors.Add(color.blue);
	}
}

void UIMesh::Init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &colorVBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertecies.GetSize() * sizeof(float), vertecies.GetPtr(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, colors.GetSize() * sizeof(float), colors.GetPtr(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	vertecies.Clear();
	colors.Clear();
}

unsigned int UIMesh::GetVAO() const
{
	return VAO;
}

unsigned int UIMesh::GetVertexArraySize() const noexcept
{
	return vertexArraySize;
}