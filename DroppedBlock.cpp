#include "DroppedBlock.h"

void DroppedBlock::Init(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT], const Texture* texture, 
	const BlockType& blockType, const glm::vec3& blockPos)
{
	mesh.GenerateCube();
	mesh.SetCubeUV(uvs[static_cast<int>(blockType)][0], uvs[static_cast<int>(blockType)][1],
		uvs[static_cast<int>(blockType)][2]);
	mesh.InitMesh();

	actor.SetMesh(&mesh);
	actor.SetTexture(const_cast<Texture*>(texture));

	glm::vec3 spawnPos = blockPos + glm::vec3(0.f, 1.f, 0.f);
	actor.SetPosition(spawnPos);
}

void DroppedBlock::Draw(Render* render)
{
	render->DrawActor(actor, false);
}