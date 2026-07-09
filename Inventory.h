#pragma once
#include "Framework.h"
#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "DroppedBlock.h"

class Inventory
{
public:
	static constexpr float INVENTORY_WIDTH = 1.5f;
	static constexpr float INVENTORY_HEIGHT = 1.f;
	static constexpr glm::vec3 START_SLOT_POS = glm::vec3(-0.33f, 0.36f, 0.f);
	static constexpr float SLOT_WIDTH = 0.125f;
	static constexpr float SLOT_HEIGHT = 0.125f;
	static constexpr float SLOT_PADDING = 0.02f;
	static constexpr int SLOT_COUNT_IN_ROW = 9;
	static constexpr int ROW_COUNT = 6;
	static constexpr int SLOTS_COUNT = SLOT_COUNT_IN_ROW * ROW_COUNT;

private:
	Window* mainWindow;
	UIMesh mesh;
	UIActor actor;
	std::array<UIMesh, SLOTS_COUNT> slotMeshes;
	std::array<UIActor, SLOTS_COUNT> slotActors;
	std::array<DroppedBlock, SLOTS_COUNT> droppedBlocks;
	int currentIndex = 0;

	void InitInventoryWindow() noexcept;
	void GenerateSlots() noexcept;

public:
	void SetMainWindow(Window* mainWindow) noexcept;
	void Init() noexcept;
	void Show(Render* render) noexcept;
	void Hide();
	void AddItem(DroppedBlock& droppedBlock, Texture* texture, UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT]);
};