#pragma once

#include "Framework.h"
#include "Camera.h"
#include "World.h"
#include "Inventory.h"
#include "DroppedBlock.h"
#include "HeldItem.h"

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

	void UpdateCamera(float deltaTime);
	void ProcessCollision(World* world) noexcept;
	void UpdatePhysics(float deltaTime) noexcept;
	void Jump() noexcept;
	void PlaceBlock(World* world, Render* render, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT],
		BlockType blockType);
	void DestroyBlock(World* world, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT], const Texture* texture,
		Render* render);
	void InitInventory(Texture* textTexture) noexcept;
	void UseInventory() noexcept;
	void DrawInventory(Render* render);
	void DrawHotBar(Render* render);
	void AddItemToInventory(DroppedBlock& droppedBlock, Texture* texture, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]);
	void ProcessHoveringForInventory(InputManager* inputManager, Render* render);
	void SetHeldItem(HeldItem* heldItem);
	void DrawHeldItem(Render* render);

	glm::vec3 GetSignMovementVector() noexcept;
	bool Colides(World* world, const glm::vec3& blockPos);
	bool ColidesAxis(World* world, const glm::vec3& blockPos);
};