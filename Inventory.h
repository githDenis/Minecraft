#pragma once
#include "Framework.h"
#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "DroppedBlock.h"
#include "Text.h"
#include "ItemDescription.h"

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
	static constexpr int MAX_ITEMS_IN_SLOT = 64;
	static constexpr Color SLOT_COLOR = Color(0.5f, 0.5f, 0.5f);

private:
	Window* mainWindow;
	UIMesh mesh;
	UIActor actor;
	std::array<UIMesh, SLOTS_COUNT> slotMeshes;
	std::array<UIActor, SLOTS_COUNT> slotActors;
	std::array<Text, SLOTS_COUNT> texts;
	std::array<ItemDescription, SLOTS_COUNT> itemDescriptions;
	std::array<DroppedBlock, SLOTS_COUNT> droppedBlocks;
	std::array<int, SLOTS_COUNT> itemsCount;

	void InitInventoryWindow() noexcept;
	void GenerateSlots(Texture* textTexture) noexcept;

public:
	void SetMainWindow(Window* mainWindow) noexcept;
	void Init(Texture* textTexture) noexcept;
	void Show(Render* render) noexcept;
	void AddItem(DroppedBlock& droppedBlock, Texture* texture, UV uvs[Chunck::BLOCKS_COUNT][Chunck::UVS_COUNT]);
	void ProcessMouseHovering(InputManager* inputManager, Render* render);
};