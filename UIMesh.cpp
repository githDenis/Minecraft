#include "UIMesh.h"

UIMesh::~UIMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void UIMesh::GenerateCrossTarget(int windowWidth, int windowHeight) noexcept
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

void UIMesh::GenerateRectangle(float width, float height, int windowWidth, int windowHeight) noexcept
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

void UIMesh::SetColor(const Color& color) noexcept
{
	useTexture = false;

	for (int i = 0; i < vertecies.GetSize(); i++)
	{
		colors.Add(color.red);
		colors.Add(color.green);
		colors.Add(color.blue);
	}
}

void UIMesh::SetRectabgleUV(const UV& uv) noexcept
{
	useTexture = true;

	float coords[] =
	{
		uv.u0, uv.v1, // v0
		uv.u0, uv.v0, // v1
		uv.u1, uv.v0, // v2

		uv.u1, uv.v0, // v3
		uv.u1, uv.v1, // v4
		uv.u0, uv.v1  // v5
	};

	static constexpr int size = sizeof(coords) / sizeof(float);
	textCoords.AddArray(coords, size);
}

void UIMesh::Init()
{
	if (!VAO)
		glGenVertexArrays(1, &VAO);
	
	if (!VBO)
		glGenBuffers(1, &VBO);
	
	if (!colorVBO)
		glGenBuffers(1, &colorVBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertecies.GetSize() * sizeof(float), vertecies.GetPtr(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);

	if (!useTexture)
	{
		glBufferData(GL_ARRAY_BUFFER, colors.GetSize() * sizeof(float), colors.GetPtr(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
		glEnableVertexAttribArray(1);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, textCoords.GetSize() * sizeof(float), textCoords.GetPtr(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
		glEnableVertexAttribArray(2);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	vertecies.Clear();

	if (colors.GetSize() > 0)
	{
		colors.Clear();
	}

	if (textCoords.GetSize() > 0)
	{
		textCoords.Clear();
	}
}

unsigned int UIMesh::GetVAO() const
{
	return VAO;
}

unsigned int UIMesh::GetVertexArraySize() const noexcept
{
	return vertexArraySize;
}

bool UIMesh::IsUseTexture() const noexcept
{
	return useTexture;
}