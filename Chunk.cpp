#include "Chunk.h"
#include "World.h"

Chunk& Chunk::operator=(Chunk&& another) noexcept
{
	std::memcpy(blockTypes, another.blockTypes, sizeof(blockTypes));
	std::memset(another.blockTypes, static_cast<unsigned char>(BlockType::BT_AIR), sizeof(another.blockTypes));
	opaqueMeshVertexOffset = another.opaqueMeshVertexOffset;
	folliageMeshVertexOffset = another.folliageMeshVertexOffset;
	transparentMeshVertexOffset = another.transparentMeshVertexOffset;
	position = another.position;
	textures = another.textures;
	opaqueMesh = std::move(another.opaqueMesh);
	folliageMesh = std::move(another.folliageMesh);
	transparentMesh = std::move(another.transparentMesh);
	return *this;
}

void Chunk::Generate() noexcept
{
	std::memset(blockTypes, static_cast<unsigned char>(BlockType::BT_AIR), sizeof(blockTypes));

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_LENGTH; z++)
		{
			float y1 = sin((x + position.x) * 0.03f) * 6 + cos((z + position.z) * 0.03f) * 6;
			float y2 = sin((x + position.x) * 0.03f) * 15 + cos((z + position.z) * 0.02f) * 15;
			float y3 = cos((x + position.x) * 0.005f) * 10 + cos((z + position.z) * 0.007f) * 20;
			float y4 = sin((x + position.x) * 0.015f) * 18 + sin((z + position.z) * 0.015f) * 20;
			float y5 = sin((x + position.x) * 0.14f) * 2 + cos((z + position.z) * 0.1f) * 2;
			float y6 = sin((x + position.x) * 0.01f) * 4 + sin((z + position.z) * 0.02f) * 5;
			int y = y1 + y2 + y3 + y4 + y5 + y6;

			constexpr int groundHeight = CHUNK_HEIGHT - GROUND_LAYER_HEIGHT;

			y += CHUNK_HEIGHT / 2;

			if (y >= groundHeight)
			{
				y = groundHeight - 1;
			}

			if (y <= 0)
			{
				y = 1;
			}

			BlockType groundBlockType = y < SAND_LAYER_HEIGHT ? BlockType::BT_SAND : BlockType::BT_GROUND;

			for (int i = 0; i < y; i++)
			{
				if (i < y - GROUND_LAYER_HEIGHT)
				{
					int coalChance = Hash(x, i, z, WORLD_SEED);
					int ironChance = Hash(x, i, z, WORLD_SEED);

					if (coalChance % World::COAL_ORE_INTENCITY == 0)
					{
						blockTypes[x][i][z] = static_cast<unsigned char>(BlockType::BT_COAL_ORE);
					}
					else if (ironChance % World::IRON_ORE_INTENCITY == 0)
					{
						blockTypes[x][i][z] = static_cast<unsigned char>(BlockType::BT_IRON_ORE);
					}
					else
					{
						blockTypes[x][i][z] = static_cast<unsigned char>(BlockType::BT_STONE);
					}
				}
				else
				{
					blockTypes[x][i][z] = static_cast<unsigned char>(groundBlockType);
				}
			}

			if (groundBlockType == BlockType::BT_GROUND)
			{
				blockTypes[x][y][z] = static_cast<unsigned char>(BlockType::BT_GROUND_GRASS);
			}
			else
			{
				blockTypes[x][y][z] = static_cast<unsigned char>(BlockType::BT_SAND);
			}

			if (groundBlockType == BlockType::BT_SAND)
			{
				for (y; y < SAND_LAYER_HEIGHT; y++)
				{
					blockTypes[x][y][z] = static_cast<unsigned char>(BlockType::BT_WATER);
				}
			}
		}
	}
}

void Chunk::GenerateTree() noexcept
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_LENGTH; z++)
		{
			int h = Hash(x + position.x, z + position.z, WORLD_SEED);

			if (h % World::TREE_INTENCITY == 0)
			{
				//Tree position
				glm::vec3 pos{ x + position.x, 0, z + position.z };

				if ((x != 0 && x != CHUNK_WIDTH - 1) && (z != 0 && z != CHUNK_LENGTH - 1))
				{
					for (int i = GROUND_LAYER_HEIGHT; i < CHUNK_HEIGHT; i++)
					{
						if (i - 1 >= 0)
						{
							BlockType topBloclType = static_cast<BlockType>(blockTypes[x][i][z]);
							BlockType downBloclType = static_cast<BlockType>(blockTypes[x][i - 1][z]);

							if (topBloclType == BlockType::BT_AIR && downBloclType == BlockType::BT_GROUND_GRASS)
							{
								pos.y = position.y + i;
								break;
							}
						}
					}
				}
				//Tree position

				if (pos.y != 0)
				{
					// Tree trunk
					int treeY = static_cast<int>(pos.y - position.y);
					int hTree = Hash(x + position.x, z + position.z, WORLD_SEED);
					int treeHeight = hTree % 10;

					if (treeHeight < 5)
					{
						treeHeight = 5;
					}
					if (treeY + treeHeight >= CHUNK_HEIGHT)
					{
						treeHeight = CHUNK_HEIGHT - treeY - 1;
					}

					for (int i = treeY; i < treeY + treeHeight; i++)
					{
						blockTypes[x][i][z] = static_cast<unsigned char>(BlockType::BT_WOOD);
					}
					// Tree trunk


					// Tree leaves
					int treeTop = treeY + treeHeight - 1;

					for (int yLeaves = treeTop; yLeaves < treeTop + 4; yLeaves++)
					{
						for (int xLeaves = -2; xLeaves <= 2; xLeaves++)
						{
							for (int zLeaves = -2; zLeaves <= 2; zLeaves++)
							{
								if ((x + xLeaves >= 0 && x + xLeaves < CHUNK_WIDTH) &&
									((z + zLeaves >= 0 && z + zLeaves < CHUNK_LENGTH)) &&
									yLeaves < CHUNK_HEIGHT)
								{
									blockTypes[x + xLeaves][yLeaves][z + zLeaves] = static_cast<unsigned char>(BlockType::BT_LEAVES);
								}
							}
						}
					}
					// Tree leaves
				}
			}
		}
	}
}

void Chunk::GenerateFolliageType(BlockType type, int intencity) noexcept
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_LENGTH; z++)
		{
			int h = Hash(x + position.x, z + position.z, WORLD_SEED);

			if (h % intencity == 0)
			{
				for (int i = GROUND_LAYER_HEIGHT; i < CHUNK_HEIGHT; i++)
				{
					if (i - 1 >= 0)
					{
						BlockType topbloclType = static_cast<BlockType>(blockTypes[x][i][z]);
						BlockType downBloclType = static_cast<BlockType>(blockTypes[x][i - 1][z]);

						if (topbloclType == BlockType::BT_AIR && downBloclType == BlockType::BT_GROUND_GRASS)
						{
							blockTypes[x][i][z] = static_cast<unsigned char>(type);
							break;
						}
					}
				}
			}
		}
	}
}

void Chunk::GenerateMeshVerticesAndTextCoords(BlockUVs& uvs) noexcept
{
	opaqueMeshVertexOffset = 0;
	folliageMeshVertexOffset = 0;
	transparentMeshVertexOffset = 0;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_LENGTH; z++)
			{
				BlockType blockType = static_cast<BlockType>(blockTypes[x][y][z]);

				if (blockType == BlockType::BT_AIR)
				{
					continue;
				}
				glm::vec3 blockPos{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) };

				BlockRenderClass blockClass = GetBlockRenderClass(glm::vec3{ blockPos });

				if (blockClass == BlockRenderClass::BC_OPAQUE)
				{
					AddCubeToMesh(glm::vec3{ blockPos }, opaqueMesh, opaqueMeshVertexOffset);
					AddCubeTextureCoords(
						uvs[static_cast<int>(blockType)][0],
						uvs[static_cast<int>(blockType)][1],
						uvs[static_cast<int>(blockType)][2],
						opaqueMesh);
				}
				else if (blockClass == BlockRenderClass::BC_FOLLIAGE)
				{
					AddCrossPlanesToMesh(glm::vec3{ blockPos }, folliageMesh, folliageMeshVertexOffset);
					AddCrossPlanesTextureCoords(uvs[static_cast<int>(blockType)][0], folliageMesh);
				}
				else
				{
					AddCubeToMesh(glm::vec3{ blockPos }, transparentMesh, transparentMeshVertexOffset);
					AddCubeTextureCoords(
						uvs[static_cast<int>(blockType)][0],
						uvs[static_cast<int>(blockType)][1],
						uvs[static_cast<int>(blockType)][2],
						transparentMesh);
				}
			}
		}
	}
}

void Chunk::AddCubeToMesh(const glm::vec3& pos, Mesh& mesh, unsigned int& vertexOffset) noexcept
{
	float vertices[] =
	{
		// FRONT
		pos.x - 0.f, pos.y - 0.f, pos.z - 0.f,
		pos.x + 1.f, pos.y - 0.f, pos.z - 0.f,
		pos.x + 1.f, pos.y + 1.f, pos.z - 0.f,
		pos.x - 0.f, pos.y + 1.f, pos.z - 0.f,


		// BACK
		pos.x - 0.f, pos.y - 0.f, pos.z + 1.f,
		pos.x - 0.f, pos.y + 1.f, pos.z + 1.f,
		pos.x + 1.f, pos.y + 1.f, pos.z + 1.f,
		pos.x + 1.f, pos.y - 0.f, pos.z + 1.f,


		// LEFT
		pos.x - 0.f, pos.y - 0.f, pos.z - 0.f,
		pos.x - 0.f, pos.y + 1.f, pos.z - 0.f,
		pos.x - 0.f, pos.y + 1.f, pos.z + 1.f,
		pos.x - 0.f, pos.y - 0.f, pos.z + 1.f,


		// RIGHT
		pos.x + 1.f, pos.y - 0.f, pos.z - 0.f,
		pos.x + 1.f, pos.y - 0.f, pos.z + 1.f,
		pos.x + 1.f, pos.y + 1.f, pos.z + 1.f,
		pos.x + 1.f, pos.y + 1.f, pos.z - 0.f,


		// BOTTOM
		pos.x - 0.f, pos.y - 0.f, pos.z - 0.f,
		pos.x - 0.f, pos.y - 0.f, pos.z + 1.f,
		pos.x + 1.f, pos.y - 0.f, pos.z + 1.f,
		pos.x + 1.f, pos.y - 0.f, pos.z - 0.f,


		// TOP
		pos.x - 0.f, pos.y + 1.f, pos.z - 0.f,
		pos.x + 1.f, pos.y + 1.f, pos.z - 0.f,
		pos.x + 1.f, pos.y + 1.f, pos.z + 1.f,
		pos.x - 0.f, pos.y + 1.f, pos.z + 1.f,
	};

	unsigned int indices[] =
	{
		// FRONT
		0 + vertexOffset, 1 + vertexOffset, 2 + vertexOffset,
		2 + vertexOffset, 3 + vertexOffset, 0 + vertexOffset,


		// BACK
		4 + vertexOffset, 5 + vertexOffset, 6 + vertexOffset,
		6 + vertexOffset, 7 + vertexOffset, 4 + vertexOffset,


		// LEFT
		8 + vertexOffset, 9 + vertexOffset, 10 + vertexOffset,
		10 + vertexOffset, 11 + vertexOffset, 8 + vertexOffset,


		// RIGHT
		12 + vertexOffset, 13 + vertexOffset, 14 + vertexOffset,
		14 + vertexOffset, 15 + vertexOffset, 12 + vertexOffset,


		// BOTTOM
		16 + vertexOffset, 17 + vertexOffset, 18 + vertexOffset,
		18 + vertexOffset, 19 + vertexOffset, 16 + vertexOffset,


		// TOP
		20 + vertexOffset, 21 + vertexOffset, 22 + vertexOffset,
		22 + vertexOffset, 23 + vertexOffset, 20 + vertexOffset,
	};

	static constexpr int vertSize = sizeof(vertices) / sizeof(vertices[0]);
	static constexpr int indSize = sizeof(indices) / sizeof(indices[0]);

	mesh.GetVertices().AddArray(vertices, vertSize);
	mesh.GetIndices().AddArray(indices, indSize);

	vertexOffset += 24;
}

void Chunk::AddCrossPlanesToMesh(const glm::vec3& pos, Mesh& mesh, unsigned int& vertexOffset) noexcept
{
	float vertices[] =
	{
		// Plane 1
		pos.x + 0.f, pos.y + 0.f, pos.z + 0.f,
		pos.x + 0.f, pos.y + 1.f, pos.z + 0.f,
		pos.x + 1.f, pos.y + 1.f, pos.z + 1.f,
		pos.x + 1.f, pos.y + 0.f, pos.z + 1.f,

		//(Opposite Plane 1)
		pos.x + 0.f, pos.y + 0.f, pos.z + 0.f,
		pos.x + 1.f, pos.y + 0.f, pos.z + 1.f,
		pos.x + 1.f, pos.y + 1.f, pos.z + 1.f,
		pos.x + 0.f, pos.y + 1.f, pos.z + 0.f,

		// Plane 2
		pos.x + 0.f, pos.y + 0.f, pos.z + 1.f,
		pos.x + 0.f, pos.y + 1.f, pos.z + 1.f,
		pos.x + 1.f, pos.y + 1.f, pos.z + 0.f,
		pos.x + 1.f, pos.y + 0.f, pos.z + 0.f,

		//(Opposite Plane 2)
		pos.x + 0.f, pos.y + 0.f, pos.z + 1.f,
		pos.x + 1.f, pos.y + 0.f, pos.z + 0.f,
		pos.x + 1.f, pos.y + 1.f, pos.z + 0.f,
		pos.x + 0.f, pos.y + 1.f, pos.z + 1.f
	};

	unsigned int indices[] =
	{
		// Plane 1
		0 + vertexOffset, 1 + vertexOffset, 2 + vertexOffset,
		2 + vertexOffset, 3 + vertexOffset, 0 + vertexOffset,

		//(Opposite Plane 1)
		8 + vertexOffset, 9 + vertexOffset, 10 + vertexOffset,
		10 + vertexOffset, 11 + vertexOffset, 8 + vertexOffset,

		// Plane 2
		4 + vertexOffset, 5 + vertexOffset, 6 + vertexOffset,
		6 + vertexOffset, 7 + vertexOffset, 4 + vertexOffset,

		//(Opposite Plane 2)
		12 + vertexOffset, 13 + vertexOffset, 14 + vertexOffset,
		14 + vertexOffset, 15 + vertexOffset, 12 + vertexOffset,
	};
	static constexpr int vertSize = sizeof(vertices) / sizeof(vertices[0]);
	static constexpr int indSize = sizeof(indices) / sizeof(indices[0]);

	mesh.GetVertices().AddArray(vertices, vertSize);
	mesh.GetIndices().AddArray(indices, indSize);

	vertexOffset += 16;
}

void Chunk::AddCubeTextureCoords(const UV& up, const UV& front, const UV& down, Mesh& mesh) noexcept
{
	float coords[] =
	{
		// FRONT
		front.u0, front.v1,
		front.u1, front.v1,
		front.u1, front.v0,
		front.u0, front.v0,


		// BACK
		front.u0, front.v1,
		front.u0, front.v0,
		front.u1, front.v0,
		front.u1, front.v1,


		// LEFT
		front.u1, front.v1,
		front.u1, front.v0,
		front.u0, front.v0,
		front.u0, front.v1,


		// RIGHT
		front.u0, front.v1,
		front.u1, front.v1,
		front.u1, front.v0,
		front.u0, front.v0,


		// BOTTOM
		down.u0, down.v1,
		down.u0, down.v0,
		down.u1, down.v0,
		down.u1, down.v1,


		// TOP
		up.u0, up.v1,
		up.u1, up.v1,
		up.u1, up.v0,
		up.u0, up.v0,
	};

	static constexpr int size = sizeof(coords) / sizeof(coords[0]);
	mesh.GetTextCoords().AddArray(coords, size);
}

void Chunk::AddCrossPlanesTextureCoords(const UV& front, Mesh& mesh) noexcept
{
	float coords[] =
	{
		// Plane 1
		front.u0, front.v1,
		front.u0, front.v0,
		front.u1, front.v0,
		front.u1, front.v1,

		//(Opposite Plane 1)
		front.u0, front.v1,
		front.u1, front.v1,
		front.u1, front.v0,
		front.u0, front.v0,

		// Plane 2
		front.u0, front.v1,
		front.u0, front.v0,
		front.u1, front.v0,
		front.u1, front.v1,

		//(Opposite Plane 2)
		front.u0, front.v1,
		front.u1, front.v1,
		front.u1, front.v0,
		front.u0, front.v0,
	};
	static constexpr int size = sizeof(coords) / sizeof(coords[0]);
	mesh.GetTextCoords().AddArray(coords, size);
}

void Chunk::InitMesh()
{
	opaqueMesh.InitMesh();
	folliageMesh.InitMesh();
	transparentMesh.InitMesh();
}

void Chunk::Draw(Render* render)
{
	opaqueActor.SetTexture(textures);
	opaqueActor.SetMesh(&opaqueMesh);
	opaqueActor.SetPosition(position);
	render->DrawActor(opaqueActor, false, false);

	folliageActor.SetTexture(textures);
	folliageActor.SetMesh(&folliageMesh);
	folliageActor.SetPosition(position);
	render->DrawActor(folliageActor, true, false);

	transparentActor.SetTexture(textures);
	transparentActor.SetMesh(&transparentMesh);
	transparentActor.SetPosition(position);
	render->DrawActor(transparentActor, false, true);
}

BlockRenderClass Chunk::GetBlockRenderClass(const glm::vec3& blockPos) const noexcept
{
	BlockType blockType = static_cast<BlockType>(blockTypes
		[static_cast<int>(blockPos.x)]
		[static_cast<int>(blockPos.y)]
		[static_cast<int>(blockPos.z)]
		);

	if (blockType >= BlockType::BT_GROUND_GRASS && blockType < BlockType::BT_GRASS)
	{
		return BlockRenderClass::BC_OPAQUE;
	}
	else if (blockType >= BlockType::BT_GRASS && blockType < BlockType::BT_WATER)
	{
		return BlockRenderClass::BC_FOLLIAGE;
	}
	else
	{
		return BlockRenderClass::BC_TRANSPARENT;
	}
}

BlockType Chunk::GetBlockType(const glm::vec3& blockPos) const noexcept
{
	if ((blockPos.x >= 0.f && blockPos.x < CHUNK_WIDTH) &&
		(blockPos.y >= 0.f && blockPos.y < CHUNK_HEIGHT) &&
		(blockPos.z >= 0.f && blockPos.z < CHUNK_LENGTH))
	{
		return static_cast<BlockType>(blockTypes
			[static_cast<int>(blockPos.x)]
			[static_cast<int>(blockPos.y)]
			[static_cast<int>(blockPos.z)]);
	}
	return BlockType::BT_AIR;
}

unsigned int Chunk::Hash(int x, int z, int seed) const noexcept
{ 
	unsigned int h = x * 374761393u + z * 668265263u + seed * 1442695041u;
	h = (h ^ (h >> 13)) * 1274126177u;
	return h ^ (h >> 16);
}

unsigned int Chunk::Hash(int x, int y, int z, int seed) const noexcept
{
	unsigned int h =
		x * 374761393u +
		y * 668265263u +
		z * 1442695041u +
		seed * 1274126177u;

	h = (h ^ (h >> 13)) * 1274126177u;
	return h ^ (h >> 16);
}