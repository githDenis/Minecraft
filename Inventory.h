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

// |0||1|
// |2||3|
struct Recipe
{
	std::array<BlockType, 4> input;
	BlockType outputBlock;
	int countCoefficient;
};

class Inventory
{
public:
	static constexpr float SLOT_WIDTH = 0.14f;
	static constexpr float SLOT_HEIGHT = 0.14f;

	static constexpr int SLOT_COUNT_IN_ROW = 9;
	static constexpr int ROW_COUNT = 4;
	static constexpr float SLOT_PADDING = 0.003f;

	static const int CRAFT_SLOT_IN_COLUMN = 2;
	static const int CRAFT_SLOT_IN_ROW = 2;
	static const int CRAFT_SLOT_COUNT = CRAFT_SLOT_IN_COLUMN * CRAFT_SLOT_IN_ROW;
	static constexpr glm::vec3 START_CRAFT_SLOT_POS = glm::vec3(-SLOT_WIDTH / 4, 0.5f, 0.f);

	static constexpr glm::vec3 CRAFT_RESULT_SLOT_POS = glm::vec3(SLOT_WIDTH, 0.5f - SLOT_HEIGHT / 2, 0.f);

	static constexpr int SLOTS_COUNT = SLOT_COUNT_IN_ROW * ROW_COUNT + CRAFT_SLOT_COUNT + 1;
	static constexpr int CRAFT_SLOT_START_INDEX = SLOT_COUNT_IN_ROW * ROW_COUNT;
	static constexpr int CRAFT_RESULT_SLOT_INDEX = SLOTS_COUNT - 1;

	static constexpr float INVENTORY_WIDTH = SLOT_WIDTH * static_cast<float>(SLOT_COUNT_IN_ROW + 1);
	static constexpr float INVENTORY_HEIGHT = SLOT_HEIGHT * 
		static_cast<float>(ROW_COUNT + CRAFT_SLOT_IN_ROW + 2);
	static constexpr Color INVENTORY_COLOR = Color(0.7f, 0.7f, 0.7f);
	static const char* CRAFTING_TEXT;
	static constexpr glm::vec3 CRAFTING_TEXT_POS = glm::vec3(-SLOT_WIDTH / 3, 0.65f, 0.f);

	static constexpr glm::vec3 INVENTORY_POS = glm::vec3(0.f, 0.15f, 0.f);
	static constexpr glm::vec3 START_SLOT_POS = glm::vec3(-0.29f, 0.2f, 0.f);
	static constexpr glm::vec3 HOT_BAR_POS = glm::vec3(-0.29f, -0.9f, 0.f);
	static constexpr int START_HOT_BAR_SLOT_INDEX = SLOT_COUNT_IN_ROW * (ROW_COUNT - 1);

	static constexpr int MAX_ITEMS_IN_SLOT = 64;
	static constexpr Color SLOT_COLOR = Color(0.5f, 0.5f, 0.5f);
	static constexpr Color CURRENT_FRAME_COLOR = Color(0.3f, 0.3f, 0.3f);

	static constexpr float DESCRIPTION_HEIGHT = Text::CHAR_HEIGHT;

private:
	Window* mainWindow;
	UIMesh mesh;
	UIActor actor;
	/*
	* 0 - 35  -  inventory slots
	* 36 - 39 -  craft slots
	* 40  -  craft result slot
	*/
	std::array<Slot, SLOTS_COUNT> slots;

	Text craftingText;

	Slot draggingSlot;
	bool isItemDragging = false;

	Slot copySlot;

	UIMesh currentItemFrameMesh;
	UIActor currentItemFrameActor;
	int currentItem = SLOT_COUNT_IN_ROW * (ROW_COUNT - 1);
	int draggingItemIndex;
	
	static const int RECIPES_COUNT = 2;
	
	std::array<Recipe, RECIPES_COUNT> recipes
	{
		Recipe {
			{ BlockType::BT_WOOD, BlockType::BT_AIR, BlockType::BT_AIR, BlockType::BT_AIR },
			BlockType::BT_PLANKS,
			4,
		},

		Recipe {
			{ BlockType::BT_PLANKS, BlockType::BT_PLANKS, BlockType::BT_PLANKS, BlockType::BT_PLANKS },
			BlockType::BT_CRAFTING_TABLE,
			1,
		},
	};


	void InitInventoryWindow() noexcept;
	void GenerateSlots(Texture* textTexture) noexcept;
	void GenerateCraftSlots(Texture* textTexture) noexcept;
	void GenerateCraftResultSlot(Texture* textTexture) noexcept;
	void InitCurrentFrame() noexcept;
	void InitDraggingSlot(Slot& slot, Texture* itemTexture, Texture* textTexture,
		UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	void InitCraftingText(Texture* textTexture) noexcept;

public:
	Slot& GetCurrentSlot() noexcept
	{
		return slots[currentItem];
	}

	BlockType GetCurrentItemBlockType() const noexcept
	{
		return slots[currentItem].block.GetBlockType();
	}

	BlockClass GetCurrentItemBlockClass() const noexcept
	{
		return slots[currentItem].block.GetBlockClass();
	}

	bool IsItemDragging() const noexcept
	{
		return isItemDragging;
	}

	void SetMainWindow(Window* mainWindow) noexcept;
	void Init(Texture* textTexture) noexcept;
	void ShowInventory(Render* render) noexcept;
	void ShowHotBar(Render* render) noexcept;
	void ShowCurrentItemFrame(Render* render) noexcept;
	void AddItem(DroppedBlock& droppedBlock, Texture* texture, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	void ProcessMouseHovering(InputManager* inputManager, Render* render) noexcept;
	void SelectLeftItem() noexcept;
	void SelectRightItem() noexcept;
	void DecreaseCurrentItem() noexcept;

	void ThrowOutItemFromInventory(InputManager* inputManger, World* world, Player* player, Texture* texture,
		UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	void ThrowOutItemFromHotbar(World* world, Player* player, Texture* texture,
		UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	void ThrowOutBlockFromSlot(const Slot& slot, World* world, Player* player, Texture* texture,
		UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;

	void ResetSlot(Slot& slot) noexcept;
	void ProcessingMouseCkick(InputManager* inputManager, Texture* itemTexture, 
		Texture* textTexture, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	void ProcessingMouseRelease(World* world, Player* player, Texture* texture, Texture* textTexture,
		UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	void UpdateDraggingItemPosition(InputManager* inputManager) noexcept;
	void ShowDraggingItem(Render* render) noexcept;
	void SwapSlots(Slot& slot1, Slot& slot2, Texture* itemTexture, Texture* textTexture,
		UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	void SplitItems(Texture* itemTexture, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;

	void CheckCrafting(Texture* itemTexture, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept;
	int GetOutputItemCountFromRecipe() noexcept;
	void UseCraftRecouses(int count) noexcept;
};