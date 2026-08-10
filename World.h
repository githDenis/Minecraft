#pragma once

#include "Framework.h"
#include "Chunk.h"
#include "DroppedBlock.h"

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

	static const int TREE_INTENCITY = 200;
	static const int GRASS_INTENCITY = 15;
	static const int RED_FLOWERS_INTENCITY = 70;
	static const int YELLOW_FLOWERS_INTENCITY = 50;
	static const int COAL_ORE_INTENCITY = 20;
	static const int IRON_ORE_INTENCITY = 32;

private:
	std::unique_ptr<Chunk[]> chunks = std::make_unique<Chunk[]>(CHUNKS_COUNT);
	Vector<BlockInfoInWorld> blocksInfo;
	Vector<class DroppedBlock> droppedBlocks;

public:
	Vector<DroppedBlock>& GetDroppedBlocksArray() noexcept
	{
		return droppedBlocks;
	}

	void GenerateChunksPositions(const glm::vec3& playerPos) noexcept;
	void GenerateChunks(Texture* textures) noexcept;
	void GenerateFolliage() noexcept;
	void ApplyChanchedBlocks(const glm::vec3& newPos) noexcept;
	void GenerateChunksMeshes(BlockUVs& uvs) noexcept;

	void RegenerateWorld(const glm::vec2& newPos, const glm::vec3& playerPos, int dx, int dy, BlockUVs& uvs) noexcept;
	void RegenerateChunkContent(Chunk& ñhunk) noexcept;
	void ReneretateChunkPosition(const glm::vec2& newPos) noexcept;
	void DrawChunks(Render* render) noexcept;
	void DrawDroppedBlocks(Render* render) noexcept;
	void SimulatePhysicsForDroppedBlocks(float deltaTime) noexcept;
	void ProcessCollisionForDroppedBlocks() noexcept;
	void ProcessRotationForDroppedBlocks(float deltaTime) noexcept;
	void ProcessCollisionWithPlayerForDroppedBlocks(class Player* player, Texture* texture, BlockUVs& uvs) noexcept;

	bool PlaceBlock(BlockUVs& uvs, Render* render, const glm::vec3& pos,glm::vec3& forwardVector, 
		BlockType blockType) noexcept;
	void DestroyBlock(BlockUVs& uvs, const Texture* texture, Render* render, const glm::vec3& pos, 
		const glm::vec3& forwardVector) noexcept;

	BlockType GetBlockType(const glm::vec3& blockPos, const glm::vec3& playerPos) const noexcept;
	BlockRenderClass GetBlockRenderClassByType(BlockType type) const noexcept;
	bool IsBlockInteractable(BlockType blockType) const noexcept;
	glm::vec3 GetBlockPos(const glm::vec3& pos, const glm::vec3& playerPos) const noexcept;
	glm::vec2 GetChunkPos(const glm::vec3& pos) const noexcept;
	int GetChunkIndex(const glm::vec3& pos, const glm::vec3& playerPos) const noexcept;
};