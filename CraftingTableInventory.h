#pragma once
#include "Framework.h"
#include "BaseInventory.h"
#include "CraftingSystem.h"

class CraftingTableInventory : public BaseInventory
{
public:
	static const int CRAFT_SLOT_IN_COLUMN = 3;
	static const int CRAFT_SLOT_IN_ROW = 3;
	static const int CRAFT_SLOT_COUNT = CRAFT_SLOT_IN_COLUMN * CRAFT_SLOT_IN_ROW;
	
	static constexpr glm::vec3 START_CRAFT_SLOT_POS = glm::vec3(-SLOT_WIDTH / 2, 0.65f, 0.f);
	static constexpr glm::vec3 CRAFT_RESULT_SLOT_POS = glm::vec3(SLOT_WIDTH * 1.5f, 0.65f - SLOT_HEIGHT, 0.f);

	static constexpr int SLOTS_COUNT = SLOT_COUNT_IN_ROW * ROW_COUNT + CRAFT_SLOT_COUNT + 1;
	static constexpr int CRAFT_SLOT_START_INDEX = SLOT_COUNT_IN_ROW * ROW_COUNT;
	static constexpr int CRAFT_RESULT_SLOT_INDEX = SLOTS_COUNT - 1;

	static constexpr float INVENTORY_WIDTH = SLOT_WIDTH * static_cast<float>(SLOT_COUNT_IN_ROW + 1);
	static constexpr float INVENTORY_HEIGHT = SLOT_HEIGHT *
		static_cast<float>(ROW_COUNT + CRAFT_SLOT_IN_ROW + 3);

	static const char* CRAFTING_TEXT;
	static constexpr glm::vec3 CRAFTING_TEXT_POS = glm::vec3(-SLOT_WIDTH / 3, 0.75f, 0.f);

	static constexpr glm::vec3 INVENTORY_POS = glm::vec3(0.f, 0.15f, 0.f);
	static constexpr glm::vec3 START_SLOT_POS = glm::vec3(-INVENTORY_WIDTH / 4.f + SLOT_PADDING / 4 * SLOT_COUNT_IN_ROW, 0.2f, 0.f);
	static constexpr int START_HOT_BAR_SLOT_INDEX = SLOT_COUNT_IN_ROW * (ROW_COUNT - 1);

private:
	CraftingSystem* craftingSystem;
	/*
	* 0 - 35  -  inventory slots
	* 36 - 44 -  craft slots
	* 45  -  craft result slot
	*/
	std::array<Slot, SLOTS_COUNT> slots;
	Text craftingText;

public:
	explicit CraftingTableInventory(CraftingSystem* craftingSystem, Window* mainWindow, 
		Texture* textTexture, float width, float height) : BaseInventory(mainWindow, textTexture, width, height)
	{
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
	virtual void SplitItems(Texture* itemTexture, BlockUVs& uvs) noexcept override;
	virtual Slot& GetSlotRefByIndex(int index) noexcept override;
	virtual const Slot& GetSlotCopyByIndex(int index) const noexcept override;

	void GenerateSlots(Texture* textTexture) noexcept;
	void GenerateCraftSlots(Texture* textTexture) noexcept;
	void GenerateCraftResultSlot(Texture* textTexture) noexcept;
	void InitDraggingSlot(Slot& slot, Texture* itemTexture, Texture* textTexture, BlockUVs& uvs) noexcept;
	void InitCraftingText(Texture* textTexture) noexcept;

	void CheckCrafting(World* world, Texture* itemTexture, BlockUVs& uvs) noexcept;
	int GetOutputItemCountFromRecipe() noexcept;
	void UseCraftRecouses(int count) noexcept;
	int GetMaxCountOfItemInCraftSlot() noexcept;
};