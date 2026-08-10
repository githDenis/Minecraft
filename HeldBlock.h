#pragma once
#include "Framework.h"
#include "HeldItem.h"

class HeldBlock : public HeldItem
{
private:
	static constexpr glm::vec3 offset = glm::vec3(-0.6f, -0.4f, -0.1f);
	static constexpr glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
	BlockType blockType;
	BlockRenderClass blockRenderClass;
	BlockUVs uvs;

public:
	void SetBlockType(BlockType blockType) noexcept;
	void SetBlockRenderClass(BlockRenderClass blockRenderClass) noexcept;
	void SetUVS(BlockUVs& uvs) noexcept;
	void Init(Texture* texture);
	virtual void Draw(Render* render) override;
};