#pragma once

#include "Framework.h"
#include "Camera.h"
#include "World.h"
#include "Inventory.h"
#include "DroppedBlock.h"
#include "PlayerHand.h"
#include "HeldBlock.h"

class Player
{
public:
	float yVelocity = 0.f;
	static constexpr float GRAVITY = 22.f;
	static constexpr float JUMP_VELOCITY = 10.f;

private:
	Camera camera;
	Inventory inventory;
	HeldItem* heldItem;
	Texture* playerHandTexture;
	PlayerHand playerHand;
	HeldBlock heldBlock;
	bool isOnGround = false;
	bool isInventoryUsing = false;

public:
	Camera& GetCamera() noexcept
	{
		return camera;
	}

	const glm::vec3& GetPosition() const noexcept
	{
		return camera.GetPosition();
	}

	const glm::vec3& GetOldPosition() const noexcept
	{
		return camera.GetOldPosition();
	}

	bool IsOnGroundState() const noexcept
	{
		return isOnGround;
	}

	bool IsInventoryUsing() const noexcept
	{
		return isInventoryUsing;
	}

	explicit Player(Window* mainWindow, InputManager* inputManager) noexcept;

	void SetHandTexture(Texture* texture) noexcept;
	void UpdateCamera(float deltaTime) noexcept;
	void ProcessCollision(World* world) noexcept;
	void UpdatePhysics(float deltaTime) noexcept;
	void Jump() noexcept;
	void PlaceBlock(World* world, Render* render, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT],
		BlockType blockType) noexcept;
	void DestroyBlock(World* world, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT], const Texture* texture,
		Render* render) noexcept;
	void InitInventory(Texture* textTexture) noexcept;
	void UseInventory() noexcept;
	void DrawInventory(Render* render) noexcept;
	void DrawHotBar(Render* render) noexcept;
	void DrawCurrentItemFrame(Render* render) noexcept;
	void AddItemToInventory(DroppedBlock& droppedBlock, Texture* texture, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	void ProcessHoveringForInventory(InputManager* inputManager, Render* render) noexcept;
	void UpdateHeldItem(UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	void StartShakingHeldItem() noexcept;
	void StopShakingHeldItem() noexcept;
	void DrawHeldItem(Render* render) noexcept;
	void SelectLeftItem() noexcept;
	void SelectRightItem() noexcept;

	glm::vec3 GetSignMovementVector() noexcept;
	bool Colides(World* world, const glm::vec3& blockPos) noexcept;
	bool ColidesAxis(World* world, const glm::vec3& blockPos) noexcept;
};