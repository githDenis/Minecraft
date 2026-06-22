#include "World.h"

void World::GenerateChuncksPositions()
{
	for (int x = 0; x < CHUNKS_HORIZONTAL_COUNT; x++)
	{
		for (int y = 0; y < CHUNKS_VERTICAL_COUNT; y++)
		{
			Vector3 pos = { x * Chunck::CHUNK_WIDTH, 0, y * Chunck::CHUNK_LENGTH };
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
	static int rightX = 0;
	static constexpr int upY = CHUNKS_VERTICAL_COUNT - 1;
	static int downY = 0;

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

unsigned short World::GetBlockType(const Vector3& blockPos) const
{
	int xChunck = (int)blockPos.x / Chunck::CHUNK_WIDTH;
	int yChunck = (int)blockPos.z / Chunck::CHUNK_LENGTH;

	int blockX = blockPos.x - xChunck * Chunck::CHUNK_WIDTH;
	int blockY = (int)blockPos.y;
	int blockZ = blockPos.z - yChunck * Chunck::CHUNK_LENGTH;

	if (blockX < 0)
	{
		blockX += Chunck::CHUNK_WIDTH;
	}

	if (blockZ < 0)
	{
		blockZ += Chunck::CHUNK_LENGTH;
	}

	Vector3 pos{ blockX, blockY, blockZ };
	return chunkcs[xChunck + yChunck * CHUNKS_HORIZONTAL_COUNT].GetBlockType(pos);
}