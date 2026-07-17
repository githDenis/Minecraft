#include "Player.h"

Player::Player(Window* mainWindow, InputManager* inputManager) noexcept
{
	camera.SetInputManager(inputManager);
	camera.SetFOV(45.f);

	inventory.SetMainWindow(mainWindow);
}

void Player::UpdateCamera(float deltaTime)
{
	camera.Update();
	camera.UpdateTranslation(deltaTime);
}

void Player::ProcessCollision(World* world) noexcept
{
	glm::vec3 newPos = camera.GetPosition() + camera.GetMovementVector();

	if (!Colides(world, newPos))
	{
		camera.SetPosition(newPos);
	}
}

void Player::UpdatePhysics(float deltaTime) noexcept
{
	glm::vec3 pos = camera.GetPosition();
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

void Player::PlaceBlock(World* world, Render* render, UV uvs[Chunck::BLOCKS_COUNT][Chunck::UVS_COUNT],
	const BlockType& blockType)
{
	glm::vec3 pos = camera.GetPosition();
	glm::vec3 forward = camera.GetFrontMovementVector();
	world->PlaceBlock(uvs, render, pos, forward, blockType);
}

void Player::DestroyBlock(World* world, UV uvs[Chunck::BLOCKS_COUNT][Chunck::UVS_COUNT], const Texture* texture,
	Render* render)
{
	glm::vec3 pos = camera.GetPosition();
	glm::vec3 forward = camera.GetFrontMovementVector();
	world->DestroyBlock(uvs, texture, render, pos, forward);
}

void Player::InitInventory(Texture* textTexture) noexcept
{
	inventory.Init(textTexture);
}

void Player::UseInventory() noexcept
{
	isInventoryUsing = !isInventoryUsing;
	camera.SetLockedState(isInventoryUsing);
}

void Player::DrawInventory(Render* render)
{
	inventory.ShowInventory(render);
}

void Player::DrawHotBar(Render* render)
{
	inventory.ShowHotBar(render);
}

void Player::AddItemToInventory(DroppedBlock& droppedBlock, Texture* texture, UV uvs[Chunck::BLOCKS_COUNT][Chunck::UVS_COUNT])
{
	inventory.AddItem(droppedBlock, texture, uvs);
}

void Player::ProcessHoveringForInventory(InputManager* inputManager, Render* render)
{
	inventory.ProcessMouseHovering(inputManager, render);
}

const Camera& Player::GetCamera() const noexcept
{
	return camera;
}

const glm::vec3& Player::GetPosition() const noexcept
{
	return camera.GetPosition();
}

const glm::vec3& Player::GetOldPosition() const noexcept
{
	return camera.GetOldPosition();
}

glm::vec3 Player::GetSignMovementVector() noexcept
{
	return camera.GetSignMovementVector();
}

bool Player::IsOnGroundState() const noexcept
{
	return isOnGround;
}

bool Player::IsInventoryUsing() const noexcept
{
	return isInventoryUsing;
}

bool Player::Colides(World* world, const glm::vec3& blockPos)
{
	int minX = floor(blockPos.x - 0.2f);
	int maxX = floor(blockPos.x + 0.2f);

	int minY = floor(blockPos.y - 1.f);
	int maxY = floor(blockPos.y);

	int minZ = floor(blockPos.z - 0.2f);
	int maxZ = floor(blockPos.z + 0.2f);

	glm::vec3 groundVec = blockPos;
	groundVec.y -= 1.8f;
	isOnGround = ColidesAxis(world, groundVec);

	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			for (int z = minZ; z <= maxZ; z++)
			{
				if (ColidesAxis(world, glm::vec3{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) }))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Player::ColidesAxis(World* world, const glm::vec3& blockPos)
{
	BlockType blockType = static_cast<BlockType>(world->GetBlockType(blockPos, GetPosition()));
	return blockType >= BlockType::BT_GROUND_GRASS && blockType <= BlockType::BT_SAND;
}