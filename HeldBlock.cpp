#include "HeldBlock.h"

void HeldBlock::SetBlockType(BlockType blockType) noexcept
{
	this->blockType = blockType;
}

void HeldBlock::SetBlockRenderClass(BlockRenderClass blockRenderClass) noexcept
{
	this->blockRenderClass = blockRenderClass;
}

void HeldBlock::SetUVS(BlockUVs& uvs) noexcept
{
	this->uvs[static_cast<unsigned char>(blockType)][0] = uvs[static_cast<unsigned char>(blockType)][0];
	this->uvs[static_cast<unsigned char>(blockType)][1] = uvs[static_cast<unsigned char>(blockType)][1];
	this->uvs[static_cast<unsigned char>(blockType)][2] = uvs[static_cast<unsigned char>(blockType)][2];
}

void HeldBlock::Init(Texture* texture)
{
	if (blockRenderClass == BlockRenderClass::BC_OPAQUE)
	{
		mesh.GenerateCubeWithOffset(offset);
		mesh.SetCubeUV(
			uvs[static_cast<unsigned char>(blockType)][0],
			uvs[static_cast<unsigned char>(blockType)][1],
			uvs[static_cast<unsigned char>(blockType)][2]);
	}
	else
	{
		mesh.GenerateCrossPlanesWithOffset(offset);
		mesh.SetCrossPlanesUV(uvs[static_cast<unsigned char>(blockType)][0]);
	}
	mesh.InitMesh();

	actor.SetMesh(&mesh);
	actor.SetTexture(texture);
	actor.SetRotation(rotation);
	actor.SetPosition(offset);
	actor.SetScale(scale);
}

void HeldBlock::Draw(Render* render)
{
	bool isFoliage = blockRenderClass == BlockRenderClass::BC_FOLLIAGE;
	render->DrawActor(actor, isFoliage, false);
}