#pragma once

#include "Framework.h"
#include "Camera.h"
#include "World.h"
#include "Inventory.h"

class Player
{
public:
	float yVelocity = 0.f;
	static constexpr float GRAVITY = 22.f;
	static constexpr float JUMP_VELOCITY = 10.f;

private:
	Camera camera;
	Inventory inventory;
	bool isOnGround = false;
	bool isInventoryUsing = false;

public:
	explicit Player(Window* mainWindow, InputManager* inputManager) noexcept;

	void UpdateCamera(float deltaTime);
	void ProcessCollision(World* world) noexcept;
	void UpdatePhysics(float deltaTime) noexcept;
	void Jump() noexcept;
	void PlaceBlock(World* world, Render* render, UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT], 
		const BlockType& blockType);
	void DestroyBlock(World* world, UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT], const Texture* texture,
		Render* render);
	void InitInventory(Texture* textTexture) noexcept;
	void UseInventory() noexcept;
	void DrawInventory(Render* render);
	void AddItemToInventory(DroppedBlock& droppedBlock, Texture* texture, UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT]);
	void ProcessHoveringForInventory(InputManager* inputManager, Render* render);

	const Camera& GetCamera() const noexcept;
	const glm::vec3& GetPosition() const noexcept;
	const glm::vec3& GetOldPosition() const noexcept;
	glm::vec3 GetSignMovementVector() noexcept;
	bool IsOnGroundState() const noexcept;
	bool IsInventoryUsing() const noexcept;
	bool Colides(World* world, const glm::vec3& blockPos);
	bool ColidesAxis(World* world, const glm::vec3& blockPos);
};