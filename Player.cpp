#include "Player.h"
#include "PlayerHand.h"
#include "HeldBlock.h"

Player::Player(Window* mainWindow, InputManager* inputManager) noexcept
{
	this->mainWindow = mainWindow;

	camera.SetInputManager(inputManager);
	camera.SetFOV(45.f);
}

void Player::SetPlayerInventory(std::unique_ptr<PlayerInventory> newInventory) noexcept
{
	inventory = std::move(newInventory);
	inventory->InitWindow();
	inventory->InitUI();
}

void Player::SetItemInventory(std::unique_ptr<BaseInventory> newInventory) noexcept
{
	itemInventory = std::move(newInventory);
	itemInventory->InitWindow();
	itemInventory->InitUI();
}

void Player::CopyItemsFromPlayerToItemInvntory() noexcept
{
	for (int i = 0; i < BaseInventory::LAST_INVENTORY_SLOT_INDEX; i++)
	{
		itemInventory->GetSlotRefByIndex(i) = inventory->GetSlotCopyByIndex(i);
	}
}

void Player::CopyItemsFromItemToPlayerInvntory() noexcept
{
	if (itemInventory.get())
	{
		for (int i = 0; i < BaseInventory::LAST_INVENTORY_SLOT_INDEX; i++)
		{
			inventory->GetSlotRefByIndex(i) = itemInventory->GetSlotCopyByIndex(i);
		}
	}
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

void Player::PlaceBlock(World* world, Render* render, BlockUVs& uvs) noexcept
{
	glm::vec3 pos = camera.GetPosition();
	glm::vec3 forward = camera.GetFrontMovementVector();

	BlockType currentBlockType = inventory->GetCurrentItemBlockType();

	if (currentBlockType != BlockType::BT_AIR)
	{
		bool placeResult = world->PlaceBlock(uvs, render, pos, forward, currentBlockType);

		if (placeResult)
		{
			inventory->DecreaseCurrentItem();
		}
	}
}

void Player::DestroyBlock(World* world, BlockUVs& uvs, const Texture* texture,
	Render* render) noexcept
{
	glm::vec3 pos = camera.GetPosition();
	glm::vec3 forward = camera.GetFrontMovementVector();
	world->DestroyBlock(uvs, texture, render, pos, forward);
}

void Player::UseInventory() noexcept
{
	isInventoryUsing = !isInventoryUsing;
	camera.SetLockedState(isInventoryUsing);
}

BlockType Player::StartLineTracing(World* world) noexcept
{
	glm::vec3 pos = camera.GetPosition();
	glm::vec3 front = camera.GetFrontMovementVector();

	for (float i = 1.f; i < INTERACT_DISTANCE; i += 0.5f)
	{
		glm::vec3 checkPos = pos + front * i;

		BlockType blockType = world->GetBlockType(checkPos, pos);

		if (blockType != BlockType::BT_AIR)
		{
			return blockType;
		}
	}
	return BlockType::BT_AIR;
}

void Player::UpdateHeldItem(BlockUVs& uvs) noexcept
{
	BlockType blockType = inventory->GetCurrentItemBlockType();

	if (blockType == BlockType::BT_AIR)
	{
		playerHand.Init(playerHandTexture);
		heldItem = &playerHand;
	}
	else
	{
		BlockRenderClass blockClass = inventory->GetCurrentItemBlockRenderClass();

		heldBlock.SetBlockType(blockType);
		heldBlock.SetBlockRenderClass(blockClass);
		heldBlock.SetUVS(uvs);
		heldBlock.Init(inventory->GetCurrentSlot().block.GetTexture());
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

bool Player::Colides(World* world, const glm::vec3& blockPos) noexcept
{
	int minX = blockPos.x - 0.2f;
	int maxX = blockPos.x + 0.2f;

	int minY = blockPos.y - 1.f;
	int maxY = blockPos.y;

	int minZ = blockPos.z - 0.2f;
	int maxZ = blockPos.z + 0.2f;

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
	BlockType blockType = world->GetBlockType(blockPos, GetPosition());
	return blockType >= BlockType::BT_GROUND_GRASS && blockType < BlockType::BT_GRASS;
}