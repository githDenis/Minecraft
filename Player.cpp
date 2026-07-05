#include "Player.h"

Player::Player(InputManager* inputManger) noexcept
{
	camera.SetInputManager(inputManger);
	camera.SetFOV(45.f);
	camera.InitMouseMoveCallback();

	inventory.InitWindow("Inventory", 400, 400);
}

void Player::UpdateCamera(float deltaTime)
{
	camera.UpdateTranslation(deltaTime);
}

void Player::ProcessCollision(World* world) noexcept
{
	Vector3 newPos = camera.GetPosition() + camera.GetMovementVector();

	if (!Colides(world, newPos))
	{
		camera.SetPosition(newPos);
	}
}

void Player::UpdatePhysics(float deltaTime) noexcept
{
	Vector3 pos = camera.GetPosition();
	yVelocity -= GRAVITY * deltaTime;

	if (isOnGround)
	{
		yVelocity = 0.f;
	}
	pos.y += yVelocity * deltaTime;
	camera.SetAxisValue('Y', pos.y);
}

void Player::Jump() noexcept
{
	isOnGround = false;
	yVelocity = JUMP_VELOCITY;
}

void Player::PlaceBlock(World* world, Render* render, UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT], 
	const BlockType& blockType)
{
	Vector3 pos = camera.GetPosition();
	Vector3 forward = camera.GetFrontMovementVector();
	world->PlaceBlock(uvs, render, pos, forward, blockType);
}

Camera& Player::GetCamera() noexcept
{
	return camera;
}

Vector3 Player::GetPosition() const noexcept
{
	return camera.GetPosition();
}

Vector3 Player::GetOldPosition() const noexcept
{
	return camera.GetOldPosition();
}

Vector3 Player::GetSignMovementVector() const noexcept
{
	return camera.GetSignMovementVector();
}

bool Player::IsOnGroundState() const noexcept
{
	return isOnGround;
}

bool Player::Colides(World* world, const Vector3& blockPos)
{
	int minX = floor(blockPos.x - 0.2f);
	int maxX = floor(blockPos.x + 0.2f);

	int minY = floor(blockPos.y - 1.f);
	int maxY = floor(blockPos.y + 1.f);

	int minZ = floor(blockPos.z - 0.2f);
	int maxZ = floor(blockPos.z + 0.2f);

	Vector3 groundVec = blockPos;
	groundVec.y -= 2;
	isOnGround = ColidesAxis(world, groundVec);

	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			for (int z = minZ; z <= maxZ; z++)
			{
				if (ColidesAxis(world, Vector3{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) }))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Player::ColidesAxis(World* world, const Vector3& blockPos)
{
	BlockType blockType = static_cast<BlockType>(world->GetBlockType(blockPos, GetPosition()));
	return blockType >= BlockType::BT_GROUND_GRASS && blockType <= BlockType::BT_SAND;
}