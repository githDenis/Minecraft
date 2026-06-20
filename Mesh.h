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
	unsigned int indeciesArraySize;

	Vector<float> vertecies;
	Vector<unsigned int> indecies;
	Vector<float> textCoords;

public:
	~Mesh();
	void GenerateCube();
	void SetUV(const UV& upUV, const UV& frontUV, const UV& downUV);

	void InitMesh();
	unsigned int GetVAO() const;
	int GetIndeciesArraySize() const noexcept;
	Vector<float>& GetVertecies() noexcept;
	Vector<unsigned int>& GetIndecies() noexcept;
	Vector<float>& GetTextCoords() noexcept;

	Mesh() noexcept = default;
	Mesh(const Mesh&) = delete;

	Mesh& operator=(const Mesh&) = delete;
	Mesh& operator=(Mesh&& another) noexcept
	{
		VAO = another.VAO;
		VBO = another.VBO;
		EBO = another.EBO;
		texCoordsVBO = another.texCoordsVBO;
		indeciesArraySize = another.indeciesArraySize;

		another.VAO = 0;
		another.VBO = 0;
		another.EBO = 0;
		another.texCoordsVBO = 0;
		another.indeciesArraySize = 0;
		return *this;
	}
};