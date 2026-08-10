#pragma once
#include "BaseInventory.h"
#include "Render.h"
#include "InputManager.h"
#include "CraftingSystem.h"

class PlayerInventory : public BaseInventory
{
public:
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
	static const char* CRAFTING_TEXT;
	static constexpr glm::vec3 CRAFTING_TEXT_POS = glm::vec3(-SLOT_WIDTH / 3, 0.65f, 0.f);

	static constexpr glm::vec3 INVENTORY_POS = glm::vec3(0.f, 0.15f, 0.f);
	static constexpr glm::vec3 START_SLOT_POS = glm::vec3(-0.29f, 0.2f, 0.f);
	static constexpr glm::vec3 HOT_BAR_POS = glm::vec3(-0.29f, -0.9f, 0.f);
	static constexpr int START_HOT_BAR_SLOT_INDEX = SLOT_COUNT_IN_ROW * (ROW_COUNT - 1);

	static constexpr Color CURRENT_FRAME_COLOR = Color(0.3f, 0.3f, 0.3f);

private:
	Player* player;
	CraftingSystem* craftingSystem;
	/*
	* 0 - 35  -  inventory slots
	* 36 - 39 -  craft slots
	* 40  -  craft result slot
	*/
	std::array<Slot, SLOTS_COUNT> slots;

	Text craftingText;

	UIMesh currentItemFrameMesh;
	UIActor currentItemFrameActor;
	int currentItem = SLOT_COUNT_IN_ROW * (ROW_COUNT - 1);

public:
	Slot& GetCurrentSlot() noexcept
	{
		return slots[currentItem];
	}

	BlockType GetCurrentItemBlockType() const noexcept
	{
		return slots[currentItem].block.GetBlockType();
	}

	BlockRenderClass GetCurrentItemBlockRenderClass() const noexcept
	{
		return slots[currentItem].block.GetBlockRenderClass();
	}

	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	explicit PlayerInventory(Player* player, CraftingSystem* craftingSystem, Window* mainWindow, 
		Texture* textTexture, float width, float height) : BaseInventory(mainWindow, textTexture, width, height)
	{
		this->player = player;
		this->craftingSystem = craftingSystem;
	}

	virtual void InitWindow() override;
	virtual void InitUI() override;
	virtual void Draw(Render* render) override;
	virtual void ProcessMouseCkick(InputManager* inputManager, Texture* itemTexture, Texture* textTexture,
		BlockUVs& uvs) noexcept override;
	virtual void ProcessMouseRelease(World* world, Texture* texture, Texture* textTexture,
		BlockUVs& uvs) noexcept override;
	virtual void ProcessMouseHovering(InputManager* inputManager, Render* render) noexcept override;
	virtual Slot& GetSlotByIndex(int index) noexcept override;
	virtual void SplitItems(Texture* itemTexture, BlockUVs& uvs) noexcept override;
	/////////////////////////////////////////////////////////////////////////////////////////////////

	void GenerateSlots(Texture* textTexture) noexcept;
	void GenerateCraftSlots(Texture* textTexture) noexcept;
	void GenerateCraftResultSlot(Texture* textTexture) noexcept;
	void InitCurrentFrame() noexcept;
	void InitDraggingSlot(Slot& slot, Texture* itemTexture, Texture* textTexture, BlockUVs& uvs) noexcept;
	void InitCraftingText(Texture* textTexture) noexcept;

	void ShowHotBar(Render* render) noexcept;
	void ShowCurrentItemFrame(Render* render) noexcept;
	void AddItem(DroppedBlock& droppedBlock, Texture* texture, BlockUVs& uvs) noexcept;
	void SelectLeftItem() noexcept;
	void SelectRightItem() noexcept;
	void DecreaseCurrentItem() noexcept;

	void ThrowOutItemFromInventory(InputManager* inputManger, World* world, Texture* texture,
		BlockUVs& uvs) noexcept;
	void ThrowOutItemFromHotbar(World* world, Texture* texture,
		BlockUVs& uvs) noexcept;
	void ThrowOutBlockFromSlot(const Slot& slot, World* world, Texture* texture,
		BlockUVs& uvs) noexcept;

	void CheckCrafting(World* world, Texture* itemTexture, BlockUVs& uvs) noexcept;
	int GetOutputItemCountFromRecipe() noexcept;
	void UseCraftRecouses(int count) noexcept;
};