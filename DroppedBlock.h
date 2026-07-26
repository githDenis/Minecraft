#pragma once
#include "Framework.h"
#include "Mesh.h"
#include "Actor.h"
#include "Render.h"
#include "Chunk.h"
#include "World.h"

class DroppedBlock
{
public:
	static constexpr float GRAVITY = 6.f;
	static constexpr float ROTATION_SPEED = 10.f;

private:
	Mesh mesh;
	Actor actor;
	BlockType blockType = BlockType::BT_AIR;
	BlockClass blockClass = BlockClass::BC_OPAQUE;
	float velocity = 0.f;
	bool isOnGround = false;
	bool isAlive = false;

public:
	void SetAliveState(bool state) noexcept
	{
		isAlive = state;
	}

	Texture* GetTexture() const noexcept
	{
		return actor.GetTexture();
	}

	const glm::vec3& GetPosition() const noexcept
	{
		return actor.GetPosition();
	}

	const Rotator& GetRotation() const noexcept
	{
		return actor.GetRotation();
	}

	BlockType GetBlockType() const noexcept
	{
		return blockType;
	}

	BlockClass GetBlockClass() const noexcept
	{
		return blockClass;
	}

	bool IsAlive() const noexcept
	{
		return isAlive;
	}

	void Init(UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT], const Texture* texture, BlockClass blockClass, 
		BlockType blockType, const glm::vec3& blockPos);
	void Draw(Render* render);
	void SimulatePhysics(float deltaTime);
	void ProcessCollision(class World* world);
	void ProcessRotation(float deltaTime);
	bool ProcessCollisionWithPlayer(class Player* player);

	const char* GetBlockText() noexcept;

	DroppedBlock() noexcept = default;
	DroppedBlock(const DroppedBlock& another) noexcept = delete;
	DroppedBlock& operator=(const DroppedBlock& another) noexcept;
	DroppedBlock& operator=(DroppedBlock&& another) noexcept;
};