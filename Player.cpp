#include "Player.h"
#include "PlayerHand.h"
#include "HeldBlock.h"

Player::Player(Window* mainWindow, InputManager* inputManager) noexcept
{
	camera.SetInputManager(inputManager);
	camera.SetFOV(45.f);

	inventory.SetMainWindow(mainWindow);
}

void Player::SetHandTexture(Texture* texture) noexcept
{
	playerHandTexture = texture;
}

void Player::UpdateCamera(float deltaTime) noexcept
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

void Player::PlaceBlock(World* world, Render* render, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	glm::vec3 pos = camera.GetPosition();
	glm::vec3 forward = camera.GetFrontMovementVector();

	BlockType currentBlockType = inventory.GetCurrentItemBlockType();

	if (currentBlockType != BlockType::BT_AIR)
	{
		world->PlaceBlock(uvs, render, pos, forward, currentBlockType);
		inventory.DecreaseCurrentItem();
	}
}

void Player::DestroyBlock(World* world, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT], const Texture* texture,
	Render* render) noexcept
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

void Player::DrawInventory(Render* render) noexcept
{
	inventory.ShowInventory(render);
}

void Player::DrawHotBar(Render* render) noexcept
{
	inventory.ShowHotBar(render);
}

void Player::DrawCurrentItemFrame(Render* render) noexcept
{
	inventory.ShowCurrentItemFrame(render);
}

void Player::AddItemToInventory(DroppedBlock& droppedBlock, Texture* texture, 
	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	inventory.AddItem(droppedBlock, texture, uvs);
}

void Player::ProcessHoveringForInventory(InputManager* inputManager, Render* render) noexcept
{
	inventory.ProcessMouseHovering(inputManager, render);
}

void Player::UpdateHeldItem(UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	BlockType blockType = inventory.GetCurrentItemBlockType();

	if (blockType == BlockType::BT_AIR)
	{
		playerHand.Init(playerHandTexture);
		heldItem = &playerHand;
	}
	else
	{
		BlockClass blockClass = inventory.GetCurrentItemBlockClass();

		heldBlock.SetBlockType(blockType);
		heldBlock.SetBlockClass(blockClass);
		heldBlock.SetUVS(uvs);
		heldBlock.Init(inventory.GetCurrentSlot().block.GetTexture());
		heldItem = &heldBlock;
	}
}

void Player::StartShakingHeldItem() noexcept
{
	if (!heldItem) return;
	heldItem->StartShaking();
}

void Player::StopShakingHeldItem() noexcept
{
	if (!heldItem) return;
	heldItem->StopShaking();
}

void Player::DrawHeldItem(Render* render) noexcept
{
	if (!heldItem) return;

	heldItem->UpdatePosition(&camera);
	heldItem->UpdateRotation(&camera);
	heldItem->Draw(render);
}

void Player::SelectLeftItem() noexcept
{
	inventory.SelectLeftItem();
}

void Player::SelectRightItem() noexcept
{
	inventory.SelectRightItem();
}

glm::vec3 Player::GetSignMovementVector() noexcept
{
	return camera.GetSignMovementVector();
}

bool Player::Colides(World* world, const glm::vec3& blockPos) noexcept
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

bool Player::ColidesAxis(World* world, const glm::vec3& blockPos) noexcept
{
	BlockType blockType = static_cast<BlockType>(world->GetBlockType(blockPos, GetPosition()));
	return blockType >= BlockType::BT_GROUND_GRASS && blockType <= BlockType::BT_SAND;
}