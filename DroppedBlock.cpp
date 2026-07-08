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

void DroppedBlock::SimulatePhysics(float deltaTime)
{
	if (!isOnGround)
	{
		glm::vec3 pos = actor.GetPosition();
		velocity -= GRAVITY * deltaTime;
		pos.y += velocity * deltaTime;
		actor.SetPosition(pos);
	}
}

void DroppedBlock::ProcessCollision(World* world)
{
	glm::vec3 checkPos = GetPosition() - glm::vec3(0.f, 0.5f, 0.f);
	BlockType blockType = world->GetBlockType(checkPos, checkPos);

	if (blockType != BlockType::BT_AIR)
	{
		velocity = 0;
		isOnGround = true;
	}
}

void DroppedBlock::ProcessRotation(float deltaTime)
{
	Rotator rotation = GetRotation();
	rotation.pitch += ROTATION_SPEED * deltaTime;
	actor.SetRotation(rotation);
}

const glm::vec3& DroppedBlock::GetPosition() const noexcept
{
	return actor.GetPosition();
}

const Rotator& DroppedBlock::GetRotation() const noexcept
{
	return actor.GetRotation();
}

DroppedBlock& DroppedBlock::operator=(DroppedBlock&& another) noexcept
{
	mesh = std::move(another.mesh);
	actor = std::move(another.actor);
	actor.SetMesh(&mesh);
	return *this;
}