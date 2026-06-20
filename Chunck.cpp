#include "Chunck.h"

void Chunck::LoadTexture(Texture* textures) noexcept
{
	this->textures = textures;
}

void Chunck::Generate()
{
	vertexOffset = 0;
	memset(blockTypes, static_cast<unsigned char>(BlockType::BT_AIR), sizeof(blockTypes));

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_LENGTH; z++)
		{
			float y1 = sin((x + position.x) * 0.03f) * 6 + cos((z + position.z) * 0.03f) * 6;
			int y2 = sin((x + position.x) * 0.03f) * 15 + cos((z + position.z) * 0.02f) * 15;
			float y3 = cos((x + position.x) * 0.005f) * 10 + cos((z + position.z) * 0.007f) * 20;
			int y4 = sin((x + position.x) * 0.015f) * 18 + sin((z + position.z) * 0.015f) * 20;
			float y5 = sin((x + position.x) * 0.14f) * 2 + cos((z + position.z) * 0.1f) * 2;
			int y6 = sin((x + position.x) * 0.01f) * 4 + sin((z + position.z) * 0.02f) * 5;
			int y = y1 + y2 + y3 + y4 + y5 + y6;

			y += CHUNK_HEIGHT / 2;

			constexpr int groundHeight = CHUNK_HEIGHT - GROUND_LAYER_HEIGHT;
			if (y >= groundHeight)
			{
				y = groundHeight - 1;
			}

			if (y <= 0)
			{
				y = 1;
			}

			BlockType groundBlockType = y < 8 ? BlockType::BT_SAND : BlockType::BT_GROUND;

			for (int i = 0; i < y; i++)
			{
				if (i < y - GROUND_LAYER_HEIGHT)
				{
					blockTypes[x][i][z] = static_cast<unsigned char>(BlockType::BT_STONE);
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
		}
	}
}

void Chunck::GenerateTree()
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_LENGTH; z++)
		{
			if (rand() % 300 < 1)
			{
				//Нахождение позиции дерева
				Vector3 pos{ x + position.x, 0, z + position.z };

				for (int i = 0; i < CHUNK_HEIGHT; i++)
				{
					if (i - 1 >= 0)
					{
						BlockType topBloclType = static_cast<BlockType>(blockTypes[x][i][z]);
						BlockType downBloclType = static_cast<BlockType>(blockTypes[x][i - 1][z]);

						if (topBloclType == BlockType::BT_AIR && downBloclType == BlockType::BT_GROUND_GRASS)
						{
							pos.y = position.y + i;
						}
					}
				}
				//Нахождение позиции дерева


				if (pos.y != 0) // Если позиция не была найдена
				{
					// Генерация ствола
					int treeY = int(pos.y - position.y);
					int treeHeight = rand() % 10;

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
						blockTypes[x][i][z] = static_cast<unsigned char>(BlockType::BT_TREE);
					}
					// Генерация ствола


					//Генерация листвы
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
					//Генерация листвы
				}
			}
		}
	}
}

void Chunck::GenerateFolliageType(const BlockType& type, int intencity)
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_LENGTH; z++)
		{
			if (rand() % intencity < 1)
			{
				//Нахождение позиции
				for (int i = 0; i < CHUNK_HEIGHT; i++)
				{
					if (i - 1 >= 0)
					{
						BlockType topbloclType = static_cast<BlockType>(blockTypes[x][i][z]);
						BlockType downBloclType = static_cast<BlockType>(blockTypes[x][i - 1][z]);

						if (topbloclType == BlockType::BT_AIR && downBloclType == BlockType::BT_GROUND_GRASS)
						{
							blockTypes[x][i][z] = static_cast<unsigned char>(type);
						}
					}
				}
				//Нахождение позиции
			}
		}
	}
}

void Chunck::GenerateMeshVerteciesAndTextCoords(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT])
{
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
				if (blockType >= BlockType::BT_GRASS && blockType <= BlockType::BT_RED_FLOWER)
				{
					AddCrossPlanesToMesh(Vector3{ float(x), float(y), float(z) });
					AddCrossPlanesTextureCoords(uvs[(int)blockType][0]);
				}
				else
				{
					AddCubeToMesh(Vector3{ float(x), float(y), float(z) });
					AddCubeTextureCoords(uvs[(int)blockType][0], uvs[(int)blockType][1], uvs[(int)blockType][2]);
				}
			}
		}
	}
}

void Chunck::AddCubeToMesh(const Vector3& pos)
{
	float vertecies[] =
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

	unsigned int indecies[] =
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

	static constexpr int vertSize = sizeof(vertecies) / sizeof(float);
	static constexpr int indSize = sizeof(indecies) / sizeof(unsigned int);

	mesh.GetVertecies().AddArray(vertecies, vertSize);
	mesh.GetIndecies().AddArray(indecies, indSize);

	vertexOffset += 24;
}

void Chunck::AddCrossPlanesToMesh(const Vector3& pos)
{
	float vertecies[] =
	{
		// Plane 1
		pos.x + 0.f, pos.y + 0.f, pos.z + 0.f,
		pos.x + 0.f, pos.y + 1.f, pos.z + 0.f,
		pos.x + 1.f, pos.y + 1.f, pos.z + 1.f,
		pos.x + 1.f, pos.y + 0.f, pos.z + 1.f,

		//(обратная Plane 1)
		pos.x + 0.f, pos.y + 0.f, pos.z + 0.f,
		pos.x + 1.f, pos.y + 0.f, pos.z + 1.f,
		pos.x + 1.f, pos.y + 1.f, pos.z + 1.f,
		pos.x + 0.f, pos.y + 1.f, pos.z + 0.f,

		// Plane 2
		pos.x + 0.f, pos.y + 0.f, pos.z + 1.f,
		pos.x + 0.f, pos.y + 1.f, pos.z + 1.f,
		pos.x + 1.f, pos.y + 1.f, pos.z + 0.f,
		pos.x + 1.f, pos.y + 0.f, pos.z + 0.f,

		//(обратная Plane 2)
		pos.x + 0.f, pos.y + 0.f, pos.z + 1.f,
		pos.x + 1.f, pos.y + 0.f, pos.z + 0.f,
		pos.x + 1.f, pos.y + 1.f, pos.z + 0.f,
		pos.x + 0.f, pos.y + 1.f, pos.z + 1.f
	};

	unsigned int indecies[] =
	{
		// Plane 1
		0 + vertexOffset, 1 + vertexOffset, 2 + vertexOffset,
		2 + vertexOffset, 3 + vertexOffset, 0 + vertexOffset,

		//(обратная Plane 1)
		8 + vertexOffset, 9 + vertexOffset, 10 + vertexOffset,
		10 + vertexOffset, 11 + vertexOffset, 8 + vertexOffset,

		// Plane 2
		4 + vertexOffset, 5 + vertexOffset, 6 + vertexOffset,
		6 + vertexOffset, 7 + vertexOffset, 4 + vertexOffset,

		//(обратная Plane 2)
		12 + vertexOffset, 13 + vertexOffset, 14 + vertexOffset,
		14 + vertexOffset, 15 + vertexOffset, 12 + vertexOffset,
	};
	static constexpr int vertSize = sizeof(vertecies) / sizeof(float);
	static constexpr int indSize = sizeof(indecies) / sizeof(unsigned int);

	mesh.GetVertecies().AddArray(vertecies, vertSize);
	mesh.GetIndecies().AddArray(indecies, indSize);

	vertexOffset += 16;
}

void Chunck::AddCubeTextureCoords(const UV& up, const UV& front, const UV& down)
{
	float cubeTextureCoords[] =
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

	static constexpr int size = sizeof(cubeTextureCoords) / sizeof(float);
	mesh.GetTextCoords().AddArray(cubeTextureCoords, size);
}

void Chunck::AddCrossPlanesTextureCoords(const UV& front)
{
	float coords[] =
	{
		// Plane 1
		front.u0, front.v1,
		front.u0, front.v0,
		front.u1, front.v0,
		front.u1, front.v1,

		//(обратная Plane 1)
		front.u0, front.v1,
		front.u1, front.v1,
		front.u1, front.v0,
		front.u0, front.v0,

		// Plane 2
		front.u0, front.v1,
		front.u0, front.v0,
		front.u1, front.v0,
		front.u1, front.v1,

		//(обратная Plane 2)
		front.u0, front.v1,
		front.u1, front.v1,
		front.u1, front.v0,
		front.u0, front.v0,
	};
	static constexpr int size = sizeof(coords) / sizeof(float);
	mesh.GetTextCoords().AddArray(coords, size);
}

void Chunck::InitMesh()
{
	mesh.InitMesh();
}

void Chunck::Draw(Render* render)
{
	actor.SetTexture(textures);
	actor.SetMesh(&mesh);
	actor.SetPosition(position);
	render->DrawActor(actor);
}

void Chunck::SetPosition(const Vector3& vector) noexcept
{
	position = vector;
}

unsigned short Chunck::GetBlockType(const Vector3& blockPos) const
{
	return blockTypes[(int)blockPos.x][(int)blockPos.y][(int)blockPos.z];
}

Vector3& Chunck::GetPosition() noexcept
{
	return position;
}