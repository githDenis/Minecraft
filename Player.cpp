#include "Player.h"

Player::Player(Camera* camera) noexcept
{
	this->camera = camera;
}

void Player::ProcessCollision(World* world)
{
	Vector3 newPos = camera->GetPosition() + camera->GetMovementVector();
	
	/*if (isOnGround)
	{
		newPos.y = camera->GetOldPosition().y;
	}

	if (!Colides(world, newPos))
	{
		camera->SetPosition(newPos);
		camera->SetOldPosition(newPos);
	}*/
	camera->SetPosition(newPos);
	camera->SetOldPosition(newPos);
}

void Player::UpdatePhysics(float deltaTime)
{
	Vector3 pos = camera->GetPosition();
	yVelocity -= GRAVITY * deltaTime;

	if (isOnGround)
	{
		yVelocity = 0.f;
	}
	pos.y += yVelocity * deltaTime;
	camera->SetPosition(pos);
}

void Player::Jump() noexcept
{
	isOnGround = false;
	yVelocity = JUMP_VELOCITY;
}

Vector3 Player::GetPosition() const noexcept
{
	return camera->GetPosition();
}

Vector3 Player::GetOldPosition() const noexcept
{
	return camera->GetOldPosition();
}

bool Player::IsOnGroundState() const noexcept
{
	return isOnGround;
}

bool Player::Colides(World* world, const Vector3& blockPos)
{
	static const int SIZE = 6;
	bool results[SIZE] = { false, false, false, false, false, false };
	Vector3 vecs[SIZE] = { blockPos, blockPos, blockPos, blockPos, blockPos, blockPos };

	vecs[(int)AxisDirection::RIGHT].x += 1;
	vecs[(int)AxisDirection::RIGHT].y -= 1;

	vecs[(int)AxisDirection::LEFT].x -= 1;
	vecs[(int)AxisDirection::LEFT].y -= 1;

	vecs[(int)AxisDirection::UP].y += 1;
	vecs[(int)AxisDirection::DOWN].y -= 2;

	vecs[(int)AxisDirection::FORWARD].z += 1;
	vecs[(int)AxisDirection::FORWARD].y -= 1;

	vecs[(int)AxisDirection::BACKWARD].z -= 1;
	vecs[(int)AxisDirection::BACKWARD].y -= 1;

	for (int i = 0; i < SIZE; i++)
	{
		results[i] = ColidesAxis(world, vecs[i]);
	}
	isOnGround = results[(int)AxisDirection::DOWN];
	
	return results[(int)AxisDirection::RIGHT] || results[(int)AxisDirection::LEFT] || 
		results[(int)AxisDirection::UP] || results[(int)AxisDirection::FORWARD] ||
		results[(int)AxisDirection::BACKWARD];
}

bool Player::ColidesAxis(World* world, const Vector3& blockPos)
{
	if ((blockPos.x >= 0) && (blockPos.y >= 0) && (blockPos.z >= 0))
	{
		BlockType blockType = static_cast<BlockType>(world->GetBlockType(blockPos));

		return blockType >= BlockType::BT_GROUND_GRASS && blockType <= BlockType::BT_SAND;
	}
	return false;
}