#pragma once

#include "Framework.h"
#include "Camera.h"
#include "World.h"
#include "PlayerInventory.h"
#include "DroppedBlock.h"
#include "PlayerHand.h"
#include "HeldBlock.h"

class Player
{
public:
	float yVelocity = 0.f;
	static constexpr float GRAVITY = 22.f;
	static constexpr float JUMP_VELOCITY = 10.f;
	static constexpr float INTERACT_DISTANCE = 5.f;

private:
	Window* mainWindow;
	Camera camera;
	std::unique_ptr<PlayerInventory> inventory;
	std::unique_ptr<BaseInventory> itemInventory;
	HeldItem* heldItem;
	Texture* playerHandTexture;
	PlayerHand playerHand;
	HeldBlock heldBlock;
	bool isOnGround = false;
	bool isInventoryUsing = false;
	bool isInteracting = false;

public:
	void SetInteractingState(bool state) noexcept
	{
		isInteracting = state;
	}

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

	bool IsInteracting() const noexcept
	{
		return isInteracting;
	}

	std::unique_ptr<PlayerInventory>& GetPlayerInventory() noexcept
	{
		return inventory;
	}

	std::unique_ptr<BaseInventory>& GetItemInventory() noexcept
	{
		return itemInventory;
	}

	explicit Player(Window* mainWindow, InputManager* inputManager) noexcept;

	void SetPlayerInventory(std::unique_ptr<PlayerInventory> newInventory) noexcept;
	void SetItemInventory(std::unique_ptr<BaseInventory> newInventory) noexcept;
	void CopyItemsFromPlayerToItemInvntory() noexcept;
	void CopyItemsFromItemToPlayerInvntory() noexcept;

	void SetHandTexture(Texture* texture) noexcept;
	void UpdateCamera(float deltaTime) noexcept;
	void ProcessCollision(World* world) noexcept;
	void UpdatePhysics(float deltaTime) noexcept;
	void Jump() noexcept;
	void PlaceBlock(World* world, Render* render, BlockUVs& uvs) noexcept;
	void DestroyBlock(World* world, BlockUVs& uvs, const Texture* texture, Render* render) noexcept;
	void UseInventory() noexcept;
	BlockType StartLineTracing(World* world) noexcept;

	void UpdateHeldItem(BlockUVs& uvs) noexcept;
	void StartShakingHeldItem() noexcept;
	void StopShakingHeldItem() noexcept;
	void DrawHeldItem(Render* render) noexcept;
	
	bool Colides(World* world, const glm::vec3& blockPos) noexcept;
	bool ColidesAxis(World* world, const glm::vec3& blockPos) noexcept;
};