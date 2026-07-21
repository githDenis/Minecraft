#include "DroppedBlock.h"
#include "Player.h"

void DroppedBlock::Init(UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT], const Texture* texture,
	BlockClass blockClass, BlockType blockType, const glm::vec3& blockPos)
{
	if (blockClass == BlockClass::BC_OPAQUE)
	{
		mesh.GenerateCube();
		mesh.SetCubeUV(uvs[static_cast<int>(blockType)][0], uvs[static_cast<int>(blockType)][1],
			uvs[static_cast<int>(blockType)][2]);
	}
	else if (blockClass == BlockClass::BC_FOLLIAGE)
	{
		mesh.GenerateCrossPlanes();
		mesh.SetCrossPlanesUV(uvs[static_cast<int>(blockType)][0]);
	}
	mesh.InitMesh();

	actor.SetMesh(&mesh);
	actor.SetTexture(const_cast<Texture*>(texture));

	glm::vec3 spawnPos = blockPos + glm::vec3(0.f, 1.f, 0.f);
	actor.SetPosition(spawnPos);

	this->blockType = blockType;
	this->blockClass = blockClass;
	isAlive = true;
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
	rotation.yaw += ROTATION_SPEED * deltaTime;
	actor.SetRotation(rotation);
}

bool DroppedBlock::ProcessCollisionWithPlayer(class Player* player)
{
	glm::vec3 pos = GetPosition();
	glm::vec3 playerPos = player->GetPosition();

	int minX = pos.x - 1.5f;
	int maxX = pos.x + 1.5f;
	int minZ = pos.z - 1.5f;
	int maxZ = pos.z + 1.5f;

	return ((playerPos.x >= minX && playerPos.x <= maxX) && (playerPos.z >= minZ && playerPos.z <= maxZ));
}

const char* DroppedBlock::GetBlockText() noexcept
{
	switch (blockType)
	{
	case BlockType::BT_GROUND_GRASS: return "Ground with grass";
	case BlockType::BT_GROUND: return "Ground";
	case BlockType::BT_TREE: return "Tree";
	case BlockType::BT_LEAVES: return "Leaves";
	case BlockType::BT_STONE: return "Stone";
	case BlockType::BT_SAND: return "Sand";
	case BlockType::BT_GRASS: return "Grass";
	case BlockType::BT_YELLOW_FLOWER: return "Yellow flower";
	case BlockType::BT_RED_FLOWER: return "Red flower";
	}
}

DroppedBlock& DroppedBlock::operator=(DroppedBlock&& another) noexcept
{
	mesh = std::move(another.mesh);
	actor = std::move(another.actor);
	isAlive = another.isAlive;
	blockType = another.blockType;
	blockClass = another.blockClass;
	strcpy(buf, another.buf);
	actor.SetMesh(&mesh);
	another.isAlive = false;
	another.blockType = BlockType::BT_AIR;
	another.blockClass = BlockClass::BC_OPAQUE;
	strcpy(another.buf, "");
	return *this;
}