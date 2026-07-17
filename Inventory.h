#pragma once
#include "Framework.h"
#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "DroppedBlock.h"
#include "Text.h"
#include "ItemDescription.h"

struct Slot
{
	UIMesh mesh;
	UIActor actor;
	Text countText;
	ItemDescription description;
	DroppedBlock block;
	int count;
};

class Inventory
{
public:
	static constexpr float SLOT_WIDTH = 0.14f;
	static constexpr float SLOT_HEIGHT = 0.14f;

	static constexpr int SLOT_COUNT_IN_ROW = 9;
	static constexpr int ROW_COUNT = 4;
	static constexpr int SLOTS_COUNT = SLOT_COUNT_IN_ROW * ROW_COUNT;
	static constexpr float SLOT_PADDING = 0.003f;

	static constexpr float INVENTORY_WIDTH = SLOT_WIDTH * static_cast<float>(SLOT_COUNT_IN_ROW + 1);
	static constexpr float INVENTORY_HEIGHT = SLOT_HEIGHT * static_cast<float>(ROW_COUNT + 1);

	static constexpr glm::vec3 INVENTORY_POS = glm::vec3(0.f, 0.3f, 0.f);
	static constexpr glm::vec3 START_SLOT_POS = glm::vec3(-0.29f, 0.55f, 0.f);
	static constexpr glm::vec3 HOT_BAR_POS = glm::vec3(-0.29f, -0.9f, 0.f);

	static constexpr int MAX_ITEMS_IN_SLOT = 64;
	static constexpr Color SLOT_COLOR = Color(0.5f, 0.5f, 0.5f);

private:
	Window* mainWindow;
	UIMesh mesh;
	UIActor actor;
	std::array<Slot, SLOTS_COUNT> slots;


	void InitInventoryWindow() noexcept;
	void GenerateSlots(Texture* textTexture) noexcept;

public:
	void SetMainWindow(Window* mainWindow) noexcept;
	void Init(Texture* textTexture) noexcept;
	void ShowInventory(Render* render) noexcept;
	void ShowHotBar(Render* render) noexcept;
	void AddItem(DroppedBlock& droppedBlock, Texture* texture, UV uvs[Chunck::BLOCKS_COUNT][Chunck::UVS_COUNT]);
	void ProcessMouseHovering(InputManager* inputManager, Render* render);
};