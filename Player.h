#pragma once

#include "Framework.h"
#include "Camera.h"
#include "World.h"

class Player
{
public:
	float yVelocity = 0.f;
	static constexpr float GRAVITY = 20.f;
	static constexpr float JUMP_VELOCITY = 10.f;

private:
	Camera* camera;
	bool isOnGround = false;

public:
	Player(Camera* camera) noexcept;

	void ProcessCollision(World* world);
	void UpdatePhysics(float deltaTime);
	void Jump() noexcept;

	Vector3 GetPosition() const noexcept;
	Vector3 GetOldPosition() const noexcept;
	bool IsOnGroundState() const noexcept;
	bool Colides(World* world, const Vector3& blockPos);
	bool ColidesAxis(World* world, const Vector3& blockPos);
};