#include "Mesh.h"

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &texCoordsVBO);
}

void Mesh::GenerateCube() noexcept
{
	static constexpr float cubeVertices[] =
	{
		// FRONT (-Z)
		-0.2f, -0.2f, -0.2f,
		0.2f, -0.2f, -0.2f,
		0.2f,  0.2f, -0.2f,
		-0.2f,  0.2f, -0.2f,

		// BACK (+Z)
		-0.2f, -0.2f,  0.2f,
		-0.2f,  0.2f,  0.2f,
		0.2f,  0.2f,  0.2f,
		0.2f, -0.2f,  0.2f,

		 // LEFT (-X)
		-0.2f, -0.2f, -0.2f,
		-0.2f,  0.2f, -0.2f,
		-0.2f,  0.2f,  0.2f,
		-0.2f, -0.2f,  0.2f,

		 // RIGHT (+X)
		0.2f, -0.2f, -0.2f,
		0.2f, -0.2f,  0.2f,
		0.2f,  0.2f,  0.2f,
		0.2f,  0.2f, -0.2f,

		// BOTTOM (-Y)
		-0.2f, -0.2f, -0.2f,
		-0.2f, -0.2f,  0.2f,
		0.2f, -0.2f,  0.2f,
		0.2f, -0.2f, -0.2f,

		// TOP (+Y)
		-0.2f,  0.2f, -0.2f,
		0.2f,  0.2f, -0.2f,
		0.2f,  0.2f,  0.2f,
		-0.2f,  0.2f,  0.2f,
	};

	static constexpr unsigned int cubeIndices[] =
	{
		// FRONT
		0, 1, 2,
		2, 3, 0,

		// BACK
		4, 5, 6,
		6, 7, 4,

		// LEFT
		8, 9, 10,
		10, 11, 8,

		// RIGHT
		12, 13, 14,
		14, 15, 12,

		// BOTTOM
		16, 17, 18,
		18, 19, 16,

		// TOP
		20, 21, 22,
		22, 23, 20,
	};

	static constexpr int vertSize = sizeof(cubeVertices) / sizeof(cubeVertices[0]);
	indicesArraySize = sizeof(cubeIndices) / sizeof(cubeIndices[0]);

	vertices.AddArray(cubeVertices, vertSize);
	indices.AddArray(cubeIndices, indicesArraySize);
}

void Mesh::GenerateCubeWithOffset(const glm::vec3& offset) noexcept
{
	float cubeVertices[] =
	{
		// FRONT (-Z)
		-0.2f + offset.x, -0.2f + offset.y, -0.2f + offset.z,
		0.2f + offset.x, -0.2f + offset.y, -0.2f + offset.z,
		0.2f + offset.x,  0.2f + offset.y, -0.2f + offset.z,
		-0.2f + offset.x,  0.2f + offset.y, -0.2f + offset.z,

		// BACK (+Z)
		-0.2f + offset.x, -0.2f + offset.y,  0.2f + offset.z,
		-0.2f + offset.x,  0.2f + offset.y,  0.2f + offset.z,
		0.2f + offset.x,  0.2f + offset.y,  0.2f + offset.z,
		0.2f + offset.x, -0.2f + offset.y,  0.2f + offset.z,

		// LEFT (-X)
	   -0.2f + offset.x, -0.2f + offset.y, -0.2f + offset.z,
	   -0.2f + offset.x,  0.2f + offset.y, -0.2f + offset.z,
	   -0.2f + offset.x,  0.2f + offset.y,  0.2f + offset.z,
	   -0.2f + offset.x, -0.2f + offset.y,  0.2f + offset.z,

	   // RIGHT (+X)
	  0.2f + offset.x, -0.2f + offset.y, -0.2f + offset.z,
	  0.2f + offset.x, -0.2f + offset.y,  0.2f + offset.z,
	  0.2f + offset.x,  0.2f + offset.y,  0.2f + offset.z,
	  0.2f + offset.x,  0.2f + offset.y, -0.2f + offset.z,

	  // BOTTOM (-Y)
	  -0.2f + offset.x, -0.2f + offset.y, -0.2f + offset.z,
	  -0.2f + offset.x, -0.2f + offset.y,  0.2f + offset.z,
	  0.2f + offset.x, -0.2f + offset.y,  0.2f + offset.z,
	  0.2f + offset.x, -0.2f + offset.y, -0.2f + offset.z,

	  // TOP (+Y)
	  -0.2f + offset.x,  0.2f + offset.y, -0.2f + offset.z,
	  0.2f + offset.x,  0.2f + offset.y, -0.2f + offset.z,
	  0.2f + offset.x,  0.2f + offset.y,  0.2f + offset.z,
	  -0.2f + offset.x,  0.2f + offset.y,  0.2f + offset.z,
	};

	static constexpr unsigned int cubeIndices[] =
	{
		// FRONT
		0, 1, 2,
		2, 3, 0,

		// BACK
		4, 5, 6,
		6, 7, 4,

		// LEFT
		8, 9, 10,
		10, 11, 8,

		// RIGHT
		12, 13, 14,
		14, 15, 12,

		// BOTTOM
		16, 17, 18,
		18, 19, 16,

		// TOP
		20, 21, 22,
		22, 23, 20,
	};

	static constexpr int vertSize = sizeof(cubeVertices) / sizeof(cubeVertices[0]);
	indicesArraySize = sizeof(cubeIndices) / sizeof(cubeIndices[0]);

	vertices.AddArray(cubeVertices, vertSize);
	indices.AddArray(cubeIndices, indicesArraySize);
}

void Mesh::SetCubeUV(const UV& upUV, const UV& frontUV, const UV& downUV) noexcept
{
	float cubeTextureCoords[] =
	{
		// FRONT
		frontUV.u0, frontUV.v1,
		frontUV.u1, frontUV.v1,
		frontUV.u1, frontUV.v0,
		frontUV.u0, frontUV.v0,

		// BACK
		frontUV.u0, frontUV.v1,
		frontUV.u0, frontUV.v0,
		frontUV.u1, frontUV.v0,
		frontUV.u1, frontUV.v1,

		// LEFT
		frontUV.u1, frontUV.v1,
		frontUV.u1, frontUV.v0,
		frontUV.u0, frontUV.v0,
		frontUV.u0, frontUV.v1,

		// RIGHT
		frontUV.u0, frontUV.v1,
		frontUV.u1, frontUV.v1,
		frontUV.u1, frontUV.v0,
		frontUV.u0, frontUV.v0,

		// BOTTOM
		downUV.u0, downUV.v1,
		downUV.u0, downUV.v0,
		downUV.u1, downUV.v0,
		downUV.u1, downUV.v1,

		// TOP
		upUV.u0, upUV.v1,
		upUV.u1, upUV.v1,
		upUV.u1, upUV.v0,
		upUV.u0, upUV.v0,
	};

	textCoords.AddArray(cubeTextureCoords, sizeof(cubeTextureCoords) / sizeof(float));
}

void Mesh::GenerateCrossPlanes() noexcept
{
	static constexpr float vertices[] =
	{
		// Plane 1
		0.f, 0.f, 0.f,
		0.f, 0.3f, 0.f,
		0.3f, 0.3f, 0.3f,
		0.3f, 0.f, 0.3f,

		//(обратная Plane 1)
		0.f, 0.f, 0.f,
		0.3f, 0.f, 0.3f,
		0.3f, 0.3f, 0.3f,
		0.f, 0.3f, 0.f,

		// Plane 2
		0.f, 0.f, 0.3f,
		0.f, 0.3f, 0.3f,
		0.3f, 0.3f, 0.f,
		0.3f, 0.f, 0.f,

		//(обратная Plane 2)
		0.f, 0.f, 0.3f,
		0.3f, 0.f, 0.f,
		0.3f, 0.3f, 0.f,
		0.f, 0.3f, 0.3f
	};

	static constexpr unsigned int indices[] =
	{
		// Plane 1
		0, 1, 2,
		2, 3, 0,

		//(обратная Plane 1)
		8, 9, 10,
		10, 11, 8,

		// Plane 2
		4, 5, 6,
		6, 7, 4,

		//(обратная Plane 2)
		12, 13, 14,
		14, 15, 12,
	};

	static constexpr int vertSize = sizeof(vertices) / sizeof(vertices[0]);
	indicesArraySize = sizeof(indices) / sizeof(indices[0]);

	this->vertices.AddArray(vertices, vertSize);
	this->indices.AddArray(indices, indicesArraySize);
}

void Mesh::SetCrossPlanesUV(const UV& front) noexcept
{
	float coords[] =
	{
		// Plane 1
		front.u0, front.v1,
		front.u0, front.v0,
		front.u1, front.v0,
		front.u1, front.v1,

		//(обратная Plane 1)
		front.u0, front.v1,
		front.u1, front.v1,
		front.u1, front.v0,
		front.u0, front.v0,

		// Plane 2
		front.u0, front.v1,
		front.u0, front.v0,
		front.u1, front.v0,
		front.u1, front.v1,

		//(обратная Plane 2)
		front.u0, front.v1,
		front.u1, front.v1,
		front.u1, front.v0,
		front.u0, front.v0,
	};
	static constexpr int size = sizeof(coords) / sizeof(coords[0]);
	textCoords.AddArray(coords, size);
}

void Mesh::InitMesh()
{
	if (VAO == 0)
	{
		glGenVertexArrays(1, &VAO);
	}

	if (VBO == 0)
	{
		glGenBuffers(1, &VBO);
	}

	if (EBO == 0)
	{
		glGenBuffers(1, &EBO);
	}

	if (texCoordsVBO == 0)
	{
		glGenBuffers(1, &texCoordsVBO);
	}

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.GetSize(), vertices.GetPtr(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.GetSize(), indices.GetPtr(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textCoords.GetSize(), textCoords.GetPtr(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indicesArraySize = indices.GetSize();

	vertices.Clear();
	indices.Clear();
	textCoords.Clear();
}

Mesh& Mesh::operator=(const Mesh& another) noexcept
{
	VAO = another.VAO;
	VBO = another.VBO;
	EBO = another.EBO;
	texCoordsVBO = another.texCoordsVBO;
	indicesArraySize = another.indicesArraySize;
	return *this;
};

Mesh& Mesh::operator=(Mesh&& another) noexcept
{
	VAO = another.VAO;
	VBO = another.VBO;
	EBO = another.EBO;
	texCoordsVBO = another.texCoordsVBO;
	indicesArraySize = another.indicesArraySize;

	another.VAO = 0;
	another.VBO = 0;
	another.EBO = 0;
	another.texCoordsVBO = 0;
	another.indicesArraySize = 0;
	return *this;
}