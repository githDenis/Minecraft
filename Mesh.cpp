#include "Mesh.h"

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &texCoordsVBO);
}

//void Mesh::GenerateCube()
//{
//	float cubeVertecies[] =
//	{
//		// FRONT (-Z)
//		-0.5f, -0.5f, -0.5f,
//		 0.5f, -0.5f, -0.5f,
//		 0.5f,  0.5f, -0.5f,
//		-0.5f,  0.5f, -0.5f,
//
//
//		// BACK (+Z)
//		-0.5f, -0.5f,  0.5f,
//		-0.5f,  0.5f,  0.5f,
//		 0.5f,  0.5f,  0.5f,
//		 0.5f, -0.5f,  0.5f,
//
//
//		 // LEFT (-X)
//		 -0.5f, -0.5f, -0.5f,
//		 -0.5f,  0.5f, -0.5f,
//		 -0.5f,  0.5f,  0.5f,
//		 -0.5f, -0.5f,  0.5f,
//
//
//		 // RIGHT (+X)
//		  0.5f, -0.5f, -0.5f,
//		  0.5f, -0.5f,  0.5f,
//		  0.5f,  0.5f,  0.5f,
//		  0.5f,  0.5f, -0.5f,
//
//
//		  // BOTTOM (-Y)
//		  -0.5f, -0.5f, -0.5f,
//		  -0.5f, -0.5f,  0.5f,
//		   0.5f, -0.5f,  0.5f,
//		   0.5f, -0.5f, -0.5f,
//
//
//		   // TOP (+Y)
//		   -0.5f,  0.5f, -0.5f,
//			0.5f,  0.5f, -0.5f,
//			0.5f,  0.5f,  0.5f,
//		   -0.5f,  0.5f,  0.5f,
//	};
//
//	unsigned int cubeIndecies[] =
//	{
//		// FRONT
//		0, 1, 2,
//		2, 3, 0,
//
//
//		// BACK
//		4, 5, 6,
//		6, 7, 4,
//
//
//		// LEFT
//		8, 9, 10,
//		10, 11, 8,
//
//
//		// RIGHT
//		12, 13, 14,
//		14, 15, 12,
//
//
//		// BOTTOM
//		16, 17, 18,
//		18, 19, 16,
//
//
//		// TOP
//		20, 21, 22,
//		22, 23, 20,
//	};
//
//	indeciesArraySize = sizeof(cubeVertecies) / sizeof(float);
//	vertecies.AddArray(cubeVertecies, indeciesArraySize);
//	indecies.AddArray(cubeIndecies, sizeof(cubeVertecies) / sizeof(unsigned int));
//}
//
//void Mesh::SetUV(const UV& upUV, const UV& frontUV, const UV& downUV)
//{
//	float cubeTextureCoords[] =
//	{
//		// FRONT
//		frontUV.u0, frontUV.v1,
//		frontUV.u1, frontUV.v1,
//		frontUV.u1, frontUV.v0,
//		frontUV.u0, frontUV.v0,
//
//		// BACK
//		frontUV.u0, frontUV.v1,
//		frontUV.u0, frontUV.v0,
//		frontUV.u1, frontUV.v0,
//		frontUV.u1, frontUV.v1,
//
//
//		// LEFT
//		frontUV.u1, frontUV.v1,
//		frontUV.u1, frontUV.v0,
//		frontUV.u0, frontUV.v0,
//		frontUV.u0, frontUV.v1,
//
//
//		// RIGHT
//		frontUV.u0, frontUV.v1,
//		frontUV.u1, frontUV.v1,
//		frontUV.u1, frontUV.v0,
//		frontUV.u0, frontUV.v0,
//
//
//		// BOTTOM
//		downUV.u0, downUV.v1,
//		downUV.u0, downUV.v0,
//		downUV.u1, downUV.v0,
//		downUV.u1, downUV.v1,
//
//
//		// TOP
//		upUV.u0, upUV.v1,
//		upUV.u1, upUV.v1,
//		upUV.u1, upUV.v0,
//		upUV.u0, upUV.v0,
//	};
//
//	textCoords.AddArray(cubeTextureCoords, sizeof(cubeTextureCoords) / sizeof(float));
//}

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertecies.GetSize(), vertecies.GetPtr(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indecies.GetSize(), indecies.GetPtr(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textCoords.GetSize(), textCoords.GetPtr(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	

	indeciesArraySize = indecies.GetSize();

	vertecies.Clear();
	indecies.Clear();
	textCoords.Clear();
}

unsigned int Mesh::GetVAO() const
{
	return VAO;
}

int Mesh::GetIndeciesArraySize() const noexcept
{
	return indeciesArraySize;
}

Vector<float>& Mesh::GetVertecies() noexcept
{
	return vertecies;
}

Vector<unsigned int>& Mesh::GetIndecies() noexcept
{
	return indecies;
}

Vector<float>& Mesh::GetTextCoords() noexcept
{
	return textCoords;
}