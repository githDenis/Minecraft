#include "World.h"

void World::GenerateChuncksPositions(const Vector3& playerPos)
{
	for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
	{
		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			Vector3 pos{
				x * Chunck::CHUNK_WIDTH + playerPos.x,
				0,
				y * Chunck::CHUNK_LENGTH + playerPos.z
			};
			chunkcs[x + y * CHUNKS_HORIZONTAL_COUNT].SetPosition(pos);
		}
	}
}

void World::GenerateChuncks(Texture* textures)
{
	for (int i = 0; i < CHUNCKS_COUNT; i++)
	{
		chunkcs[i].LoadTexture(textures);
		chunkcs[i].Generate();
	}
}

void World::GenerateFolliage()
{
	for (int i = 0; i < CHUNCKS_COUNT; i++)
	{
		chunkcs[i].GenerateTree();
		chunkcs[i].GenerateFolliageType(BlockType::BT_GRASS, 15);
		chunkcs[i].GenerateFolliageType(BlockType::BT_YELLOW_FLOWER, 70);
		chunkcs[i].GenerateFolliageType(BlockType::BT_RED_FLOWER, 50);
	}
}

void World::GenerateChuncksMeshes(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT])
{
	for (int i = 0; i < CHUNCKS_COUNT; i++)
	{
		chunkcs[i].GenerateMeshVerteciesAndTextCoords(uvs);
		chunkcs[i].InitMesh();
	}
}

void World::RegenerateWorld(const Vector2& newPos, int dx, int dy, UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT])
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
				chunkcs[x + y * CHUNKS_HORIZONTAL_COUNT] = std::move(chunkcs[(x + 1) + y * CHUNKS_HORIZONTAL_COUNT]);
			}
		}

		ReneretateChunckPosition(newPos);

		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			RegenerateChunckContent(chunkcs[leftX + y * CHUNKS_HORIZONTAL_COUNT]);
			chunkcs[leftX + y * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerteciesAndTextCoords(uvs);
			chunkcs[leftX + y * CHUNKS_HORIZONTAL_COUNT].InitMesh();
		}
	}

	if (dx < 0)
	{
		for (int x = CHUNKS_HORIZONTAL_COUNT - 1; x > 0; x--)
		{
			for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
			{
				chunkcs[x + y * CHUNKS_HORIZONTAL_COUNT] = std::move(chunkcs[(x - 1) + y * CHUNKS_HORIZONTAL_COUNT]);
			}
		}

		ReneretateChunckPosition(newPos);

		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			RegenerateChunckContent(chunkcs[rightX + y * CHUNKS_HORIZONTAL_COUNT]);
			chunkcs[rightX + y * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerteciesAndTextCoords(uvs);
			chunkcs[rightX + y * CHUNKS_HORIZONTAL_COUNT].InitMesh();
		}
	}

	if (dy < 0)
	{
		for (int y = CHUNKS_VERTICAL_COUNT - 1; y > 0; y--)
		{
			for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
			{
				chunkcs[x + y * CHUNKS_HORIZONTAL_COUNT] = std::move(chunkcs[x + (y - 1) * CHUNKS_HORIZONTAL_COUNT]);
			}
		}

		ReneretateChunckPosition(newPos);

		for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
		{
			RegenerateChunckContent(chunkcs[x + downY * CHUNKS_HORIZONTAL_COUNT]);
			chunkcs[x + downY * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerteciesAndTextCoords(uvs);
			chunkcs[x + downY * CHUNKS_HORIZONTAL_COUNT].InitMesh();
		}
	}

	if (dy > 0)
	{
		for (int y = 0; y < CHUNKS_VERTICAL_COUNT - 1; y++)
		{
			for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
			{
				chunkcs[x + y * CHUNKS_HORIZONTAL_COUNT] = std::move(chunkcs[x + (y + 1) * CHUNKS_HORIZONTAL_COUNT]);
			}
		}

		ReneretateChunckPosition(newPos);

		for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
		{
			RegenerateChunckContent(chunkcs[x + upY * CHUNKS_HORIZONTAL_COUNT]);
			chunkcs[x + upY * CHUNKS_HORIZONTAL_COUNT].GenerateMeshVerteciesAndTextCoords(uvs);
			chunkcs[x + upY * CHUNKS_HORIZONTAL_COUNT].InitMesh();
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

void World::ReneretateChunckPosition(const Vector2& newPos)
{
	for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
	{
		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			Vector3 pos = {
				(x + newPos.x) * Chunck::CHUNK_WIDTH,
				0,
				(y + newPos.y) * Chunck::CHUNK_LENGTH
			};
			chunkcs[x + y * CHUNKS_HORIZONTAL_COUNT].SetPosition(pos);
		}
	}
}

void World::DrawChunck(Render* render, int index)
{
	chunkcs[index].Draw(render);
}

unsigned char World::GetBlockType(const Vector3& blockPos, const Vector3& playerPos) const
{
	int xChunck = floor(blockPos.x / Chunck::CHUNK_WIDTH);
	int zChunck = floor(blockPos.z / Chunck::CHUNK_LENGTH);
	
	int xPlayerChunck = floor(playerPos.x / Chunck::CHUNK_WIDTH);
	int zPlayerChunck = floor(playerPos.z / Chunck::CHUNK_LENGTH);

	int localX = xChunck - xPlayerChunck + DRAW_CHUNK_RADIUS;
	int localZ = zChunck - zPlayerChunck + DRAW_CHUNK_RADIUS;

	std::cout << "X: " << localX << "  Y: " << localZ << '\n';

	int blockX = floor(blockPos.x) - xChunck * Chunck::CHUNK_WIDTH;
	int blockY = floor(blockPos.y);
	int blockZ = floor(blockPos.z) - zChunck * Chunck::CHUNK_LENGTH;
	
	int chunckIndex = localX + localZ * CHUNKS_HORIZONTAL_COUNT;

	Vector3 pos{
		static_cast<float>(blockX),
		static_cast<float>(blockY),
		static_cast<float>(blockZ)
	};
	return chunkcs[chunckIndex].GetBlockType(pos);
}