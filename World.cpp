#include "World.h"
#include "DroppedBlock.h"
#include "Player.h"

void World::GenerateChunksPositions(const glm::vec3& playerPos) noexcept
{
	for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
	{
		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			glm::vec3 pos{
				x * Chunk::CHUNK_WIDTH + playerPos.x,
				0,
				y * Chunk::CHUNK_LENGTH + playerPos.z
			};
			chunks[x + y * CHUNKS_HORIZONTAL_COUNT].SetPosition(pos);
		}
	}
}

void World::GenerateChunks(Texture* textures) noexcept
{
	for (int i = 0; i < CHUNKS_COUNT; i++)
	{
		chunks[i].LoadTexture(textures);
		chunks[i].Generate();
	}
}

void World::GenerateFolliage() noexcept
{
	for (int i = 0; i < CHUNKS_COUNT; i++)
	{
		chunks[i].GenerateTree();
		chunks[i].GenerateFolliageType(BlockType::BT_GRASS, 15);
		chunks[i].GenerateFolliageType(BlockType::BT_YELLOW_FLOWER, 70);
		chunks[i].GenerateFolliageType(BlockType::BT_RED_FLOWER, 50);
	}
}

void World::ApplyChanchedBlocks(const glm::vec3& newPos) noexcept
{
	for (int i = 0; i < blocksInfo.GetSize(); i++)
	{
		glm::vec2 changedChunkPos{ GetChunkPos(blocksInfo[i].pos) };
		glm::vec2 playerChunkPos{ GetChunkPos(newPos) };

		int minXChunk = playerChunkPos.x - DRAW_CHUNK_RADIUS;
		int maxXChunk = playerChunkPos.x + DRAW_CHUNK_RADIUS;
		int minZChunk = playerChunkPos.y - DRAW_CHUNK_RADIUS;
		int maxZChunk = playerChunkPos.y + DRAW_CHUNK_RADIUS;

		int chunkIndex = GetChunkIndex(blocksInfo[i].pos, newPos);

		if (chunkIndex >= 0 && chunkIndex <= CHUNKS_COUNT)
		{
			if ((changedChunkPos.x >= minXChunk && changedChunkPos.x <= maxXChunk) &&
				(changedChunkPos.y >= minZChunk && changedChunkPos.y <= maxZChunk))
			{
				glm::vec3 pos{ GetBlockPos(blocksInfo[i].pos, newPos) };
				chunks[chunkIndex].PlaceBlock(pos, blocksInfo[i].blockType);
			}
		}
	}
}

void World::GenerateChunksMeshes(UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	for (int i = 0; i < CHUNKS_COUNT; i++)
	{
		chunks[i].GenerateMeshVerticesAndTextCoords(uvs);
		chunks[i].InitMesh();
	}
}

void World::RegenerateWorld(const glm::vec2& newPos, const glm::vec3& playerPos, int dx, int dy,
	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	static constexpr int leftX = CHUNKS_HORIZONTAL_COUNT - 1;
	static const int rightX = 0;
	static constexpr int upY = CHUNKS_VERTICAL_COUNT - 1;
	static const int downY = 0;

	if (dx > 0)
	{
		for (int x = 0; x < leftX; x++)
		{
			for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
			{
				chunks[x + y * CHUNKS_HORIZONTAL_COUNT] = std::move(chunks[(x + 1) + y * CHUNKS_HORIZONTAL_COUNT]);
			}
		}

		ReneretateChunkPosition(newPos);

		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			RegenerateChunkContent(chunks[leftX + y * CHUNKS_HORIZONTAL_COUNT]);

			if (blocksInfo.GetSize() > 0)
			{
				ApplyChanchedBlocks(playerPos);
			}
			chunks[leftX + y * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerticesAndTextCoords(uvs);
			chunks[leftX + y * CHUNKS_HORIZONTAL_COUNT].InitMesh();
		}
	}

	if (dx < 0)
	{
		for (int x = CHUNKS_HORIZONTAL_COUNT - 1; x > 0; x--)
		{
			for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
			{
				chunks[x + y * CHUNKS_HORIZONTAL_COUNT] = std::move(chunks[(x - 1) + y * CHUNKS_HORIZONTAL_COUNT]);
			}
		}

		ReneretateChunkPosition(newPos);

		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			RegenerateChunkContent(chunks[rightX + y * CHUNKS_HORIZONTAL_COUNT]);

			if (blocksInfo.GetSize() > 0)
			{
				ApplyChanchedBlocks(playerPos);
			}
			chunks[rightX + y * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerticesAndTextCoords(uvs);
			chunks[rightX + y * CHUNKS_HORIZONTAL_COUNT].InitMesh();
		}
	}

	if (dy < 0)
	{
		for (int y = CHUNKS_VERTICAL_COUNT - 1; y > 0; y--)
		{
			for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
			{
				chunks[x + y * CHUNKS_HORIZONTAL_COUNT] = std::move(chunks[x + (y - 1) * CHUNKS_HORIZONTAL_COUNT]);
			}
		}

		ReneretateChunkPosition(newPos);

		for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
		{
			RegenerateChunkContent(chunks[x + downY * CHUNKS_HORIZONTAL_COUNT]);

			if (blocksInfo.GetSize() > 0)
			{
				ApplyChanchedBlocks(playerPos);
			}
			chunks[x + downY * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerticesAndTextCoords(uvs);
			chunks[x + downY * CHUNKS_HORIZONTAL_COUNT].InitMesh();
		}
	}

	if (dy > 0)
	{
		for (int y = 0; y < CHUNKS_VERTICAL_COUNT - 1; y++)
		{
			for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
			{
				chunks[x + y * CHUNKS_HORIZONTAL_COUNT] = std::move(chunks[x + (y + 1) * CHUNKS_HORIZONTAL_COUNT]);
			}
		}

		ReneretateChunkPosition(newPos);

		for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
		{
			RegenerateChunkContent(chunks[x + upY * CHUNKS_HORIZONTAL_COUNT]);

			if (blocksInfo.GetSize() > 0)
			{
				ApplyChanchedBlocks(playerPos);
			}
			chunks[x + upY * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerticesAndTextCoords(uvs);
			chunks[x + upY * CHUNKS_HORIZONTAL_COUNT].InitMesh();
		}
	}
}

void World::RegenerateChunkContent(Chunk& chunk) noexcept
{
	chunk.Generate();
	chunk.GenerateTree();
	chunk.GenerateFolliageType(BlockType::BT_GRASS, 15);
	chunk.GenerateFolliageType(BlockType::BT_YELLOW_FLOWER, 70);
	chunk.GenerateFolliageType(BlockType::BT_RED_FLOWER, 50);
}

void World::ReneretateChunkPosition(const glm::vec2& newPos) noexcept
{
	for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
	{
		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			glm::vec3 pos = {
				(x + newPos.x) * Chunk::CHUNK_WIDTH,
				0,
				(y + newPos.y) * Chunk::CHUNK_LENGTH
			};
			chunks[x + y * CHUNKS_HORIZONTAL_COUNT].SetPosition(pos);
		}
	}
}

void World::DrawChunks(Render* render) noexcept
{
	for (int i = 0; i < World::CHUNKS_COUNT; i++)
	{
		chunks[i].Draw(render);
	}
}

void World::DrawDroppedBlocks(Render* render) noexcept
{
	for (int i = 0; i < droppedBlocks.GetSize(); i++)
	{
		if (droppedBlocks[i].IsAlive())
		{
			droppedBlocks[i].Draw(render);
		}
	}
}

void World::SimulatePhysicsForDroppedBlocks(float deltaTime) noexcept
{
	for (int i = 0; i < droppedBlocks.GetSize(); i++)
	{
		if (droppedBlocks[i].IsAlive())
		{
			droppedBlocks[i].SimulatePhysics(deltaTime);
		}
	}
}

void World::ProcessCollisionForDroppedBlocks() noexcept
{
	for (int i = 0; i < droppedBlocks.GetSize(); i++)
	{
		if (droppedBlocks[i].IsAlive())
		{
			droppedBlocks[i].ProcessCollision(this);
		}
	}
}

void World::ProcessRotationForDroppedBlocks(float deltaTime) noexcept
{
	for (int i = 0; i < droppedBlocks.GetSize(); i++)
	{
		if (droppedBlocks[i].IsAlive())
		{
			droppedBlocks[i].ProcessRotation(deltaTime);
		}
	}
}

void World::ProcessCollisionWithPlayerForDroppedBlocks(class Player* player, Texture* texture,
	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	for (int i = 0; i < droppedBlocks.GetSize(); i++)
	{
		if (droppedBlocks[i].IsAlive())
		{
			if (droppedBlocks[i].ProcessCollisionWithPlayer(player))
			{
				player->AddItemToInventory(droppedBlocks[i], texture, uvs);
			}
		}
	}
}

void World::PlaceBlock(UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT], Render* render, const glm::vec3& pos,
	glm::vec3& forwardVector, BlockType blockType) noexcept
{
	glm::vec3 start = pos;
	glm::vec3 direction = forwardVector;

	for (float i = 1.f; i < BREAK_BLOCK_DISTANCE; i++)
	{
		glm::vec3 checkPos = start + direction * i;
		glm::vec3 blockPos{ GetBlockPos(checkPos, pos) };
		int chunkIndex = GetChunkIndex(checkPos, pos);

		if (chunks[chunkIndex].GetBlockType(blockPos) != BlockType::BT_AIR &&
			chunks[chunkIndex].GetBlockType(blockPos) != BlockType::BT_WATER)
		{
			float k = blockType == BlockType::BT_AIR ? i : i - 1;
			checkPos = start + direction * k;

			glm::vec3 placePos{ GetBlockPos(checkPos, pos) };
			chunkIndex = GetChunkIndex(checkPos, pos);

			chunks[chunkIndex].PlaceBlock(placePos, blockType);
			chunks[chunkIndex].GenerateMeshVerticesAndTextCoords(uvs);
			chunks[chunkIndex].InitMesh();
			chunks[chunkIndex].Draw(render);

			BlockInfoInWorld blockInfo{
				.pos = checkPos,
				.blockType = blockType,
			};
			blocksInfo.Add(blockInfo);
			return;
		}
	}
}

void World::DestroyBlock(UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT], const Texture* texture, Render* render,
	const glm::vec3& pos, const glm::vec3& forwardVector) noexcept
{
	glm::vec3 start = pos;
	glm::vec3 direction = forwardVector;

	for (float i = 1.f; i < BREAK_BLOCK_DISTANCE; i++)
	{
		glm::vec3 checkPos = start + direction * i;
		glm::vec3 blockPos{ GetBlockPos(checkPos, pos) };
		int chunkIndex = GetChunkIndex(checkPos, pos);

		if (chunks[chunkIndex].GetBlockType(blockPos) != BlockType::BT_AIR &&
			chunks[chunkIndex].GetBlockType(blockPos) != BlockType::BT_WATER)
		{
			checkPos = start + direction * i;

			glm::vec3 placePos{ GetBlockPos(checkPos, pos) };
			chunkIndex = GetChunkIndex(checkPos, pos);

			BlockClass blockClass = static_cast<BlockClass>(chunks[chunkIndex].GetBlockClass(blockPos));
			BlockType blockType = static_cast<BlockType>(chunks[chunkIndex].GetBlockType(blockPos));

			chunks[chunkIndex].PlaceBlock(placePos, BlockType::BT_AIR);
			chunks[chunkIndex].GenerateMeshVerticesAndTextCoords(uvs);
			chunks[chunkIndex].InitMesh();
			chunks[chunkIndex].Draw(render);

			BlockInfoInWorld blockInfo{
				.pos = checkPos,
				.blockType = BlockType::BT_AIR,
			};
			blocksInfo.Add(blockInfo);

			DroppedBlock droppedBlock;
			droppedBlock.Init(uvs, texture, blockClass, blockType, checkPos);
			droppedBlocks.Add(std::move(droppedBlock));
			return;
		}
	}
}

BlockType World::GetBlockType(const glm::vec3& blockPos, const glm::vec3& playerPos) const noexcept
{
	glm::vec3 pos{ GetBlockPos(blockPos, playerPos) };
	int chunkIndex = GetChunkIndex(blockPos, playerPos);
	return chunks[chunkIndex].GetBlockType(pos);
}

glm::vec3 World::GetBlockPos(const glm::vec3& pos, const glm::vec3& playerPos) const noexcept
{
	int xChunk = floor(pos.x / Chunk::CHUNK_WIDTH);
	int zChunk = floor(pos.z / Chunk::CHUNK_LENGTH);

	int xPlayerChunk = floor(playerPos.x / Chunk::CHUNK_WIDTH);
	int zPlayerChunk = floor(playerPos.z / Chunk::CHUNK_LENGTH);

	int localX = xChunk - xPlayerChunk + DRAW_CHUNK_RADIUS;
	int localZ = zChunk - zPlayerChunk + DRAW_CHUNK_RADIUS;

	int blockX = floor(pos.x) - xChunk * Chunk::CHUNK_WIDTH;
	int blockY = floor(pos.y);
	int blockZ = floor(pos.z) - zChunk * Chunk::CHUNK_LENGTH;

	return glm::vec3{ static_cast<float>(blockX), static_cast<float>(blockY), static_cast<float>(blockZ) };
}

glm::vec2 World::GetChunkPos(const glm::vec3& pos) const noexcept
{
	return glm::vec2{ floor(pos.x / Chunk::CHUNK_WIDTH), floor(pos.z / Chunk::CHUNK_LENGTH) };
}

int World::GetChunkIndex(const glm::vec3& pos, const glm::vec3& playerPos) const noexcept
{
	int xChunk = floor(pos.x / Chunk::CHUNK_WIDTH);
	int zChunk = floor(pos.z / Chunk::CHUNK_LENGTH);

	int xPlayerChunk = floor(playerPos.x / Chunk::CHUNK_WIDTH);
	int zPlayerChunk = floor(playerPos.z / Chunk::CHUNK_LENGTH);

	int localX = xChunk - xPlayerChunk + DRAW_CHUNK_RADIUS;
	int localZ = zChunk - zPlayerChunk + DRAW_CHUNK_RADIUS;

	return localX + localZ * CHUNKS_HORIZONTAL_COUNT;
}