#pragma once

#include "Framework.h"
#include "Chunck.h"

struct BlockInfoInWorld
{
	glm::vec3 pos;
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
	void GenerateChuncksPositions(const glm::vec3& playerPos);
	void GenerateChuncks(Texture* textures);
	void GenerateFolliage();
	void ApplyChanchedBlocks(const glm::vec3& newPos);
	void GenerateChuncksMeshes(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT]);
	
	void RegenerateWorld(const glm::vec2& newPos, const glm::vec3& playerPos, int dx, int dy,
		UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT]);
	void RegenerateChunckContent(Chunck& chunck);
	void ReneretateChunckPosition(const glm::vec2& newPos);
	void DrawChunck(Render* render, int index);
	
	void PlaceBlock(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT], Render* render, const glm::vec3& pos,
		glm::vec3& forwardVector, const BlockType& blockType);

	unsigned char GetBlockType(const glm::vec3& blockPos, const glm::vec3& playerPos) const;

private:
	glm::vec3 GetBlockPos(const glm::vec3& pos, const glm::vec3& playerPos) const noexcept;
	glm::vec2 GetChunckPos(const glm::vec3& pos) const noexcept;
	int GetChunckIndex(const glm::vec3& pos, const glm::vec3& playerPos) const noexcept;
};