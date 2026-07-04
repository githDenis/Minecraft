#pragma once

#include "Framework.h"
#include "Mesh.h"
#include "Texture.h"
#include "Render.h"

enum class BlockType : unsigned char {
	BT_GROUND_GRASS,
	BT_GROUND,
	BT_TREE,
	BT_LEAVES,
	BT_STONE,
	BT_SAND,
	BT_GRASS,
	BT_YELLOW_FLOWER,
	BT_RED_FLOWER,
	BT_WATER,
	BT_AIR,
};

enum class BlockClass : unsigned char {
	BC_OPAQUE,
	BC_FOLLIAGE,
	BC_TRANSPARENT
};

class Chunck
{
public:
	static const int BLOCKS_TYPES_COUNT = static_cast<int>(BlockType::BT_AIR);
	static const int UVS_COUNT = 3;
	static const int CHUNK_WIDTH = 16;
	static const int CHUNK_HEIGHT = 32;
	static const int CHUNK_LENGTH = 16;
	static const int GROUND_LAYER_HEIGHT = 10;
	static const int WORLD_SEED = 42;

private:
	unsigned char blockTypes[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH];
	unsigned int opaqueMeshVertexOffset;
	unsigned int transparentMeshVertexOffset;

	Mesh opaqueMesh;
	Mesh transparentMesh;

	Vector3 position;
	Texture* textures;
	Actor opaqueActor;
	Actor transparentActor;

public:
	void LoadTexture(Texture* textures) noexcept;
	void Generate();
	void GenerateTree();
	void GenerateFolliageType(const BlockType& type, int intencity);
	void GenerateMeshVerteciesAndTextCoords(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT]);
	void AddCubeToMesh(const Vector3& pos, Mesh& mesh, unsigned int& vertexOffset);
	void AddCrossPlanesToMesh(const Vector3& pos, Mesh& mesh);
	void AddCubeTextureCoords(const UV& up, const UV& front, const UV& down, Mesh& mesh);
	void AddCrossPlanesTextureCoords(const UV& front);
	void InitMesh();
	void Draw(Render* render);
	void SetPosition(const Vector3& vector) noexcept;
	void PlaceBlock(const Vector3& blockPos, const BlockType& blockType);
	void SetBlockType(const Vector3& blockPos, const BlockType& newType);

	BlockClass GetBlockClass(const Vector3& blockPos) const noexcept;
	unsigned char GetBlockType(const Vector3& blockPos) const noexcept;
	Vector3& GetPosition() noexcept;
	unsigned int Hash(int x, int z, int seed) const noexcept;

	Chunck() noexcept = default;
	Chunck(const Chunck&) = delete;
	
	Chunck& operator=(const Chunck&) = delete;
	Chunck& operator=(Chunck&& another) noexcept
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (int z = 0; z < CHUNK_LENGTH; z++)
				{
					blockTypes[x][y][z] = another.blockTypes[x][y][z];
				}
			}
		}

		opaqueMeshVertexOffset = another.opaqueMeshVertexOffset;
		transparentMeshVertexOffset = another.transparentMeshVertexOffset;
		position = another.position;
		textures = another.textures;
		opaqueMesh = std::move(another.opaqueMesh);
		transparentMesh = std::move(another.transparentMesh);

		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (int z = 0; z < CHUNK_LENGTH; z++)
				{
					another.blockTypes[x][y][z] = static_cast<unsigned char>(BlockType::BT_AIR);
				}
			}
		}
		return *this;
	}
};