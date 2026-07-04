#pragma once

#include "Framework.h"
#include "Chunck.h"

struct BlockInfoInWorld
{
	Vector3 pos;
	BlockType blockType;
};

class World
{
public:
	static const int DRAW_CHUNK_RADIUS = 4;
	static constexpr int CHUNKS_VERTICAL_COUNT = DRAW_CHUNK_RADIUS * 2 + 1;
	static constexpr int CHUNKS_HORIZONTAL_COUNT = DRAW_CHUNK_RADIUS * 2 + 1;
	static constexpr int CHUNKS_COUNT = CHUNKS_VERTICAL_COUNT * CHUNKS_HORIZONTAL_COUNT;
	static constexpr int CENTRAL_CHUNK_INDEX = (CHUNKS_COUNT - 1) / 2;
	static constexpr float BREAK_BLOCK_DISTANCE = 10.f;

private:
	std::unique_ptr<Chunck[]> chunks = std::make_unique<Chunck[]>(CHUNKS_COUNT);
	Vector<BlockInfoInWorld> blocksInfo;

public:
	void GenerateChuncksPositions(const Vector3& playerPos);
	void GenerateChuncks(Texture* textures);
	void GenerateFolliage();
	void ApplyChanchedBlocks(const Vector3& newPos);
	void GenerateChuncksMeshes(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT]);
	
	void RegenerateWorld(const Vector2& newPos, const Vector3& playerPos, int dx, int dy, 
		UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT]);
	void RegenerateChunckContent(Chunck& chunck);
	void ReneretateChunckPosition(const Vector2& newPos);
	void DrawChunck(Render* render, int index);
	
	void PlaceBlock(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT], Render* render, const Vector3& pos,
		Vector3& forwardVector, const BlockType& blockType);

	unsigned char GetBlockType(const Vector3& blockPos, const Vector3& playerPos) const;

private:
	Vector3 GetBlockPos(const Vector3& pos, const Vector3& playerPos) const noexcept;
	Vector2 GetChunckPos(const Vector3& pos) const noexcept;
	int GetChunckIndex(const Vector3& pos, const Vector3& playerPos) const noexcept;
};