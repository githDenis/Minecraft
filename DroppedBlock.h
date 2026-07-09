#pragma once
#include "Framework.h"
#include "Mesh.h"
#include "Actor.h"
#include "Render.h"
#include "Chunck.h"
#include "World.h"

class DroppedBlock
{
public:
	static constexpr float GRAVITY = 6.f;
	static constexpr float ROTATION_SPEED = 8.f;

private:
	Mesh mesh;
	Actor actor;
	BlockType blockType;
	float velocity = 0.f;
	bool isOnGround = false;
	bool isAlive = false;

public:
	void Init(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT], const Texture* texture, const BlockType& blockType,
		const glm::vec3& blockPos);
	void Draw(Render* render);
	void SimulatePhysics(float deltaTime);
	void ProcessCollision(class World* world);
	void ProcessRotation(float deltaTime);
	bool ProcessCollisionWithPlayer(class Player* player);
	void SetAliveState(bool state) noexcept;

	const glm::vec3& GetPosition() const noexcept;
	const Rotator& GetRotation() const noexcept;
	const BlockType& GetBlockType() const noexcept;
	bool IsAlive() const noexcept;

	DroppedBlock() noexcept = default;
	DroppedBlock(const DroppedBlock& another) noexcept = delete;
	DroppedBlock& operator=(const DroppedBlock& another) noexcept = delete;
	DroppedBlock& operator=(DroppedBlock&& another) noexcept;
};