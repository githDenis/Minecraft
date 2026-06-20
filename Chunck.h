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
	BT_AIR,
};

class Chunck
{
public:
	static const int BLOCKS_TYPES_COUNT = static_cast<int>(BlockType::BT_AIR);
	static const int UVS_COUNT = 3;
	static const int CHUNK_WIDTH = 16;
	static const int CHUNK_HEIGHT = 48;
	static const int CHUNK_LENGTH = 16;
	static const int GROUND_LAYER_HEIGHT = 10;

private:
	unsigned char blockTypes[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH];
	unsigned int vertexOffset;

	Mesh mesh;

	Vector3 position;
	Texture* textures;
	Actor actor;

public:
	void LoadTexture(Texture* textures) noexcept;
	void Generate();
	void GenerateTree();
	void GenerateFolliageType(const BlockType& type, int intencity);
	void GenerateMeshVerteciesAndTextCoords(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT]);
	void AddCubeToMesh(const Vector3& pos);
	void AddCrossPlanesToMesh(const Vector3& pos);
	void AddCubeTextureCoords(const UV& up, const UV& front, const UV& down);
	void AddCrossPlanesTextureCoords(const UV& front);
	void InitMesh();
	void Draw(Render* render);
	void SetPosition(const Vector3& vector) noexcept;

	unsigned short GetBlockType(const Vector3& blockPos) const;
	Vector3& GetPosition() noexcept;

	Chunck() noexcept = default;
	Chunck(const Chunck&) = delete;
	
	Chunck& operator=(const Chunck&) = delete;
	Chunck& operator=(Chunck&& another) noexcept
	{
		memcpy(blockTypes, another.blockTypes, sizeof(blockTypes));
		vertexOffset = another.vertexOffset;
		position = another.position;
		textures = another.textures;
		mesh = std::move(another.mesh);
		return *this;
	}
};