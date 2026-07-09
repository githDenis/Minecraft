#include "World.h"
#include "DroppedBlock.h"
#include "Player.h"

void World::GenerateChuncksPositions(const glm::vec3& playerPos)
{
	for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
	{
		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			glm::vec3 pos{
				x * Chunck::CHUNK_WIDTH + playerPos.x,
				0,
				y * Chunck::CHUNK_LENGTH + playerPos.z
			};
			chunks[x + y * CHUNKS_HORIZONTAL_COUNT].SetPosition(pos);
		}
	}
}

void World::GenerateChuncks(Texture* textures)
{
	for (int i = 0; i < CHUNKS_COUNT; i++)
	{
		chunks[i].LoadTexture(textures);
		chunks[i].Generate();
	}
}

void World::GenerateFolliage()
{
	for (int i = 0; i < CHUNKS_COUNT; i++)
	{
		chunks[i].GenerateTree();
		chunks[i].GenerateFolliageType(BlockType::BT_GRASS, 15);
		chunks[i].GenerateFolliageType(BlockType::BT_YELLOW_FLOWER, 70);
		chunks[i].GenerateFolliageType(BlockType::BT_RED_FLOWER, 50);
	}
}

void World::ApplyChanchedBlocks(const glm::vec3& newPos)
{
	for (int i = 0; i < blocksInfo.GetSize(); i++)
	{
		glm::vec3 pos{ GetBlockPos(blocksInfo[i].pos, newPos) };
		glm::vec2 changedChunkPos{ GetChunckPos(blocksInfo[i].pos) };
		glm::vec2 playerChunkPos{ GetChunckPos(newPos) };

		int minXChunk = playerChunkPos.x - DRAW_CHUNK_RADIUS;
		int maxXChunk = playerChunkPos.x + DRAW_CHUNK_RADIUS;
		int minZChunk = playerChunkPos.y - DRAW_CHUNK_RADIUS;
		int maxZChunk = playerChunkPos.y + DRAW_CHUNK_RADIUS;

		int chunckIndex = GetChunckIndex(blocksInfo[i].pos, newPos);

		if (chunckIndex >= 0 && chunckIndex <= CHUNKS_COUNT)
		{
			if ((changedChunkPos.x >= minXChunk && changedChunkPos.x <= maxXChunk) &&
				(changedChunkPos.y >= minZChunk && changedChunkPos.y <= maxZChunk))
			{
				chunks[chunckIndex].SetBlockType(pos, blocksInfo[i].blockType);
			}
		}
	}
}

void World::GenerateChuncksMeshes(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT])
{
	for (int i = 0; i < CHUNKS_COUNT; i++)
	{
		chunks[i].GenerateMeshVerteciesAndTextCoords(uvs);
		chunks[i].InitMesh();
	}
}

void World::RegenerateWorld(const glm::vec2& newPos, const glm::vec3& playerPos, int dx, int dy,
	UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT])
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

		ReneretateChunckPosition(newPos);

		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			RegenerateChunckContent(chunks[leftX + y * CHUNKS_HORIZONTAL_COUNT]);
			
			if (blocksInfo.GetSize() > 0)
			{
				ApplyChanchedBlocks(playerPos);
			}
			chunks[leftX + y * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerteciesAndTextCoords(uvs);
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

		ReneretateChunckPosition(newPos);

		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			RegenerateChunckContent(chunks[rightX + y * CHUNKS_HORIZONTAL_COUNT]);
			
			if (blocksInfo.GetSize() > 0)
			{
				ApplyChanchedBlocks(playerPos);
			}
			chunks[rightX + y * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerteciesAndTextCoords(uvs);
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

		ReneretateChunckPosition(newPos);

		for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
		{
			RegenerateChunckContent(chunks[x + downY * CHUNKS_HORIZONTAL_COUNT]);
			
			if (blocksInfo.GetSize() > 0)
			{
				ApplyChanchedBlocks(playerPos);
			}
			chunks[x + downY * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerteciesAndTextCoords(uvs);
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

		ReneretateChunckPosition(newPos);

		for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
		{
			RegenerateChunckContent(chunks[x + upY * CHUNKS_HORIZONTAL_COUNT]);
			
			if (blocksInfo.GetSize() > 0)
			{
				ApplyChanchedBlocks(playerPos);
			}
			chunks[x + upY * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerteciesAndTextCoords(uvs);
			chunks[x + upY * CHUNKS_HORIZONTAL_COUNT].InitMesh();
		}
	}
}

void World::RegenerateChunckContent(Chunck& chunck)
{
	chunck.Generate();
	chunck.GenerateTree();
	chunck.GenerateFolliageType(BlockType::BT_GRASS, 15);
	chunck.GenerateFolliageType(BlockType::BT_YELLOW_FLOWER, 70);
	chunck.GenerateFolliageType(BlockType::BT_RED_FLOWER, 50);
}

void World::ReneretateChunckPosition(const glm::vec2& newPos)
{
	for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
	{
		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			glm::vec3 pos = {
				(x + newPos.x) * Chunck::CHUNK_WIDTH,
				0,
				(y + newPos.y) * Chunck::CHUNK_LENGTH
			};
			chunks[x + y * CHUNKS_HORIZONTAL_COUNT].SetPosition(pos);
		}
	}
}

void World::DrawChuncks(Render* render)
{
	for (int i = 0; i < World::CHUNKS_COUNT; i++)
	{
		chunks[i].Draw(render);
	}
}

void World::DrawDroppedBlocks(Render* render)
{
	for (int i = 0; i < droppedBlocks.GetSize(); i++)
	{
		if (droppedBlocks[i].IsAlive())
		{
			droppedBlocks[i].Draw(render);
		}
	}
}

void World::SimulatePhysicsForDroppedBlocks(float deltaTime)
{
	for (int i = 0; i < droppedBlocks.GetSize(); i++)
	{
		if (droppedBlocks[i].IsAlive())
		{
			droppedBlocks[i].SimulatePhysics(deltaTime);
		}
	}
}

void World::ProcessCollisionForDroppedBlocks()
{
	for (int i = 0; i < droppedBlocks.GetSize(); i++)
	{
		if (droppedBlocks[i].IsAlive())
		{
			glm::vec3 droppedBlockPos{ droppedBlocks[i].GetPosition() };
			glm::vec3 posInChunck{ GetBlockPos(droppedBlockPos, droppedBlockPos) };
			int chunckIndex = GetChunckIndex(droppedBlockPos, droppedBlockPos);

			droppedBlocks[i].ProcessCollision(this);
		}
	}
}

void World::ProcessRotationForDroppedBlocks(float deltaTime)
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
	UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT])
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

void World::PlaceBlock(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT], Render* render, const glm::vec3& pos,
	glm::vec3& forwardVector, const BlockType& blockType)
{
	glm::vec3 start = pos;
	glm::vec3 direction = forwardVector;

	for (float i = 1.f; i < BREAK_BLOCK_DISTANCE; i++)
	{
		glm::vec3 checkPos = start + direction * i;
		glm::vec3 blockPos{ GetBlockPos(checkPos, pos) };
		int chunckIndex = GetChunckIndex(checkPos, pos);

		if (chunks[chunckIndex].GetBlockType(blockPos) != BlockType::BT_AIR &&
			chunks[chunckIndex].GetBlockType(blockPos) != BlockType::BT_WATER)
		{
			float k = blockType == BlockType::BT_AIR ? i : i - 1;
			checkPos = start + direction * k;
			
			glm::vec3 placePos{ GetBlockPos(checkPos, pos) };
			chunckIndex = GetChunckIndex(checkPos, pos);

			chunks[chunckIndex].PlaceBlock(placePos, blockType);
			chunks[chunckIndex].GenerateMeshVerteciesAndTextCoords(uvs);
			chunks[chunckIndex].InitMesh();
			chunks[chunckIndex].Draw(render);

			BlockInfoInWorld blockInfo{
				.pos = checkPos,
				.blockType = blockType,
			};
			blocksInfo.Add(blockInfo);
			return;
		}
	}
}

void World::DestroyBlock(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT], const Texture* texture, Render* render,
	const glm::vec3& pos, const glm::vec3& forwardVector)
{
	glm::vec3 start = pos;
	glm::vec3 direction = forwardVector;

	for (float i = 1.f; i < BREAK_BLOCK_DISTANCE; i++)
	{
		glm::vec3 checkPos = start + direction * i;
		glm::vec3 blockPos{ GetBlockPos(checkPos, pos) };
		int chunckIndex = GetChunckIndex(checkPos, pos);

		if (chunks[chunckIndex].GetBlockType(blockPos) != BlockType::BT_AIR &&
			chunks[chunckIndex].GetBlockType(blockPos) != BlockType::BT_WATER)
		{
			checkPos = start + direction * i;

			glm::vec3 placePos{ GetBlockPos(checkPos, pos) };
			chunckIndex = GetChunckIndex(checkPos, pos);
			
			BlockType blockType = static_cast<BlockType>(chunks[chunckIndex].GetBlockType(blockPos));
			chunks[chunckIndex].PlaceBlock(placePos, BlockType::BT_AIR);
			chunks[chunckIndex].GenerateMeshVerteciesAndTextCoords(uvs);
			chunks[chunckIndex].InitMesh();
			chunks[chunckIndex].Draw(render);

			BlockInfoInWorld blockInfo {
				.pos = checkPos,
				.blockType = BlockType::BT_AIR,
			};
			blocksInfo.Add(blockInfo);

			DroppedBlock droppedBlock;
			droppedBlock.Init(uvs, texture, blockType, checkPos);
			droppedBlocks.Add(std::move(droppedBlock));
			return;
		}
	}
}

BlockType World::GetBlockType(const glm::vec3& blockPos, const glm::vec3& playerPos) const
{
	glm::vec3 pos{ GetBlockPos(blockPos, playerPos) };
	int chunckIndex = GetChunckIndex(blockPos, playerPos);
	return chunks[chunckIndex].GetBlockType(pos);
}

glm::vec3 World::GetBlockPos(const glm::vec3& pos, const glm::vec3& playerPos) const noexcept
{
	int xChunck = floor(pos.x / Chunck::CHUNK_WIDTH);
	int zChunck = floor(pos.z / Chunck::CHUNK_LENGTH);

	int xPlayerChunck = floor(playerPos.x / Chunck::CHUNK_WIDTH);
	int zPlayerChunck = floor(playerPos.z / Chunck::CHUNK_LENGTH);

	int localX = xChunck - xPlayerChunck + DRAW_CHUNK_RADIUS;
	int localZ = zChunck - zPlayerChunck + DRAW_CHUNK_RADIUS;

	int blockX = floor(pos.x) - xChunck * Chunck::CHUNK_WIDTH;
	int blockY = floor(pos.y);
	int blockZ = floor(pos.z) - zChunck * Chunck::CHUNK_LENGTH;

	return glm::vec3{ static_cast<float>(blockX), static_cast<float>(blockY), static_cast<float>(blockZ) };
}

glm::vec2 World::GetChunckPos(const glm::vec3& pos) const noexcept
{
	return glm::vec2{ floor(pos.x / Chunck::CHUNK_WIDTH), floor(pos.z / Chunck::CHUNK_LENGTH) };
}

int World::GetChunckIndex(const glm::vec3& pos, const glm::vec3& playerPos) const noexcept
{
	int xChunck = floor(pos.x / Chunck::CHUNK_WIDTH);
	int zChunck = floor(pos.z / Chunck::CHUNK_LENGTH);

	int xPlayerChunck = floor(playerPos.x / Chunck::CHUNK_WIDTH);
	int zPlayerChunck = floor(playerPos.z / Chunck::CHUNK_LENGTH);

	int localX = xChunck - xPlayerChunck + DRAW_CHUNK_RADIUS;
	int localZ = zChunck - zPlayerChunck + DRAW_CHUNK_RADIUS;

	return localX + localZ * CHUNKS_HORIZONTAL_COUNT;
}