#include "HeldBlock.h"

void HeldBlock::SetBlockType(BlockType blockType) noexcept
{
	this->blockType = blockType;
}

void HeldBlock::SetBlockClass(BlockClass blockClass) noexcept
{
	this->blockClass = blockClass;
}

void HeldBlock::SetUVS(UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	this->uvs[static_cast<unsigned char>(blockType)][0] = uvs[static_cast<unsigned char>(blockType)][0];
	this->uvs[static_cast<unsigned char>(blockType)][1] = uvs[static_cast<unsigned char>(blockType)][1];
	this->uvs[static_cast<unsigned char>(blockType)][2] = uvs[static_cast<unsigned char>(blockType)][2];
}

void HeldBlock::Init(Texture* texture)
{
	if (blockClass == BlockClass::BC_OPAQUE)
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