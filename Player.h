#pragma once

#include "Framework.h"
#include "Camera.h"
#include "World.h"

class Player
{
public:
	float yVelocity = 0.f;
	static constexpr float GRAVITY = 22.f;
	static constexpr float JUMP_VELOCITY = 10.f;

private:
	Camera* camera;
	bool isOnGround = false;

public:
	explicit Player(Camera* camera) noexcept;

	void ProcessCollision(World* world) noexcept;
	void UpdatePhysics(float deltaTime) noexcept;
	void Jump() noexcept;
	void PlaceBlock(World* world, Render* render, UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT], 
		const BlockType& blockType);

	Vector3 GetPosition() const noexcept;
	Vector3 GetOldPosition() const noexcept;
	Vector3 GetSignMovementVector() const noexcept;
	bool IsOnGroundState() const noexcept;
	bool Colides(World* world, const Vector3& blockPos);
	bool ColidesAxis(World* world, const Vector3& blockPos);
};