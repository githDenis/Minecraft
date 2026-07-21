#pragma once
#include "Framework.h"
#include "HeldItem.h"

class HeldBlock : public HeldItem
{
private:
	static constexpr glm::vec3 offset = glm::vec3(-0.6f, -0.4f, -0.1f);
	static constexpr glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
	BlockType blockType;
	BlockClass blockClass;
	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT];

public:
	void SetBlockType(BlockType blockType) noexcept;
	void SetBlockClass(BlockClass blockClass) noexcept;
	void SetUVS(UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	void Init(Texture* texture);
};