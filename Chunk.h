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

class Chunk
{
public:
	static const int BLOCKS_COUNT = static_cast<int>(BlockType::BT_AIR);
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

	glm::vec3 position;
	Texture* textures;
	Actor opaqueActor;
	Actor transparentActor;

public:
	void LoadTexture(Texture* textures) noexcept
	{
		this->textures = textures;
	}

	void SetPosition(const glm::vec3& vector) noexcept
	{
		position = vector;
	}

	void PlaceBlock(const glm::vec3& blockPos, BlockType blockType) noexcept
	{
		blockTypes[static_cast<int>(blockPos.x)][static_cast<int>(blockPos.y)][static_cast<int>(blockPos.z)] =
			static_cast<unsigned char>(blockType);
	}

	const glm::vec3& GetPosition() const noexcept
	{
		return position;
	}

	void Generate() noexcept;
	void GenerateTree() noexcept;
	void GenerateFolliageType(BlockType type, int intencity) noexcept;
	void GenerateMeshVerticesAndTextCoords(UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	void AddCubeToMesh(const glm::vec3& pos, Mesh& mesh, unsigned int& vertexOffset) noexcept;
	void AddCrossPlanesToMesh(const glm::vec3& pos, Mesh& mesh) noexcept;
	void AddCubeTextureCoords(const UV& up, const UV& front, const UV& down, Mesh& mesh) noexcept;
	void AddCrossPlanesTextureCoords(const UV& front) noexcept;
	void InitMesh();
	void Draw(Render* render);

	BlockClass GetBlockClass(const glm::vec3& blockPos) const noexcept;
	BlockType GetBlockType(const glm::vec3& blockPos) const noexcept;
	unsigned int Hash(int x, int z, int seed) const noexcept;

	Chunk() noexcept = default;
	Chunk(const Chunk&) = delete;

	Chunk& operator=(const Chunk&) = delete;
	Chunk& operator=(Chunk&& another) noexcept;
};