#pragma once

#include "Framework.h"
#include "Chunck.h"

class World
{
public:
	static const int DRAW_CHUNK_RADIUS = 4;
	static const int CHUNKS_VERTICAL_COUNT = DRAW_CHUNK_RADIUS * 2 + 1;
	static const int CHUNKS_HORIZONTAL_COUNT = DRAW_CHUNK_RADIUS * 2 + 1;
	static constexpr int CHUNCKS_COUNT = CHUNKS_VERTICAL_COUNT * CHUNKS_HORIZONTAL_COUNT;

private:
	std::unique_ptr<Chunck[]> chunkcs = std::make_unique<Chunck[]>(CHUNCKS_COUNT);

public:
	void GenerateChuncksPositions();
	void GenerateChuncks(Texture* textures);
	void GenerateFolliage();
	void GenerateChuncksMeshes(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT]);

	void RegenerateWorld(const Vector2& newPos, int dx, int dy, UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT]);
	void RegenerateChunckContent(Chunck& chunck);
	void ReneretateChunckPosition(const Vector2& newPos);
	void DrawChunck(Render* render, int index);

	unsigned short GetBlockType(const Vector3& blockPos) const;
};