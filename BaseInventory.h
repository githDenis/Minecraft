#pragma once
#include "Framework.h"
#include "Window.h"
#include "UIActor.h"
#include "DroppedBlock.h"
#include "Text.h"
#include "ItemDescription.h"
#include <string>

struct Slot
{
	UIActor actor;
	Text countText;
	ItemDescription description;
	DroppedBlock block;
	int count;

	Slot() noexcept = default;

	Slot& operator=(const Slot& another) noexcept
	{
		actor = another.actor;
		countText = another.countText;
		description = another.description;
		block = another.block;
		count = another.count;
		return *this;
	}
};

class BaseInventory
{
public:
	static constexpr Color INVENTORY_COLOR = Color(0.7f, 0.7f, 0.7f);

	static constexpr float SLOT_WIDTH = 0.14f;
	static constexpr float SLOT_HEIGHT = 0.14f;
	static constexpr int SLOT_COUNT_IN_ROW = 9;
	static constexpr int ROW_COUNT = 4;
	static constexpr float SLOT_PADDING = 0.01f;
	static constexpr Color SLOT_COLOR = Color(0.5f, 0.5f, 0.5f);
	static const int MAX_ITEMS_IN_SLOT = 64;
	static constexpr float DESCRIPTION_HEIGHT = Text::CHAR_HEIGHT;
	static const int LAST_INVENTORY_SLOT_INDEX = 35;

protected:
	Window* mainWindow;
	Texture* textTexture;
	UIActor actor;

	float width;
	float height;

	Slot copySlot;
	Slot draggingSlot;
	bool isItemDragging = false;
	int draggingItemIndex;

public:
	explicit BaseInventory(Window* mainWindow, Texture* textTexture, float width, float height);

	bool IsItemDragging() const noexcept
	{
		return isItemDragging;
	}

	void UpdateDraggingItemPosition(InputManager* inputManager) noexcept;
	void ShowDraggingItem(Render* render) noexcept;
	void ResetSlot(Slot& slot) noexcept;
	void SwapSlots(Slot& slot1, Slot& slot2, Texture* itemTexture, Texture* textTexture, BlockUVs& uvs) noexcept;

	virtual void InitWindow() = 0;
	virtual void InitUI() = 0;
	virtual void Draw(Render* render) = 0;
	virtual void ProcessMouseCkick(InputManager* inputManager, Texture* itemTexture, Texture* textTexture,
		BlockUVs& uvs) noexcept = 0;
	virtual void ProcessMouseRelease(World* world, Texture* texture, Texture* textTexture,
		BlockUVs& uvs) noexcept = 0;
	virtual void ProcessMouseHovering(InputManager* inputManager, Render* render) noexcept = 0;
	virtual void SplitItems(Texture* itemTexture, BlockUVs& uvs) noexcept = 0;
	virtual Slot& GetSlotRefByIndex(int index) noexcept = 0;
	virtual const Slot& GetSlotCopyByIndex(int index) const noexcept = 0;
};