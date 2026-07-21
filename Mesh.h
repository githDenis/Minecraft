#pragma once

#include "Framework.h"
#include "Structs.h"

class Mesh
{
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int texCoordsVBO;
	unsigned int indicesArraySize;

	Vector<float> vertices;
	Vector<unsigned int> indices;
	Vector<float> textCoords;

public:
	unsigned int GetVAO() const
	{
		return VAO;
	}

	int GetIndicesArraySize() const noexcept
	{
		return indicesArraySize;
	}

	Vector<float>& GetVertices() noexcept
	{
		return vertices;
	}

	Vector<unsigned int>& GetIndices() noexcept
	{
		return indices;
	}

	Vector<float>& GetTextCoords() noexcept
	{
		return textCoords;
	}

	~Mesh();
	void GenerateCube() noexcept;
	void GenerateCubeWithOffset(const glm::vec3& offset) noexcept;
	void SetCubeUV(const UV& upUV, const UV& frontUV, const UV& downUV) noexcept;

	void GenerateCrossPlanes() noexcept;
	void GenerateCrossPlanesWithOffset(const glm::vec3& offset) noexcept;
	void SetCrossPlanesUV(const UV& front) noexcept;

	void InitMesh();

	Mesh() noexcept = default;
	Mesh(const Mesh&) = delete;

	Mesh& operator=(const Mesh& another) noexcept;
	Mesh& operator=(Mesh&& another) noexcept;
};