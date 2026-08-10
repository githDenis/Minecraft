#include "BaseInventory.h"
#include "Render.h"
#include <string>

BaseInventory::BaseInventory(Window* mainWindow, Texture* textTexture, float width, float height)
{
	this->mainWindow = mainWindow;
	this->textTexture = textTexture;
	this->width = width;
	this->height = height;
}

void BaseInventory::UpdateDraggingItemPosition(InputManager* inputManager) noexcept
{
	if (isItemDragging)
	{
		glm::vec2 NDCPos = inputManager->GetMouseNDC();
		glm::vec3 pos(NDCPos.x, NDCPos.y, 0.f);
		draggingSlot.actor.SetPosition(pos);
	}
}

void BaseInventory::ShowDraggingItem(Render* render) noexcept
{
	if (isItemDragging)
	{
		render->DrawUIActor(draggingSlot.actor, GL_TRIANGLES);
	}
}

void BaseInventory::ResetSlot(Slot& slot) noexcept
{
	slot.count = 0;
	slot.countText.SetText("");
	slot.description.SetText("");
	slot.description.ResetMesh();
	slot.mesh.GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	slot.mesh.SetColor(SLOT_COLOR);
	slot.mesh.Init();
	slot.actor.SetMesh(&slot.mesh);
	slot.block = DroppedBlock();
}

void BaseInventory::SwapSlots(Slot& slot1, Slot& slot2, Texture* itemTexture, Texture* textTexture,
	BlockUVs& uvs) noexcept
{
	std::string textCount;
	char* blockDescription;
	float desctiptionWidth;

	textCount = std::to_string(draggingSlot.count);
	blockDescription = const_cast<char*>(draggingSlot.block.GetBlockText());
	desctiptionWidth = strlen(blockDescription) * Text::CHAR_WIDTH / 2;

	// ------------------------ Copy Slot ------------------------ //
	copySlot.countText.SetMainWindow(mainWindow);
	copySlot.countText.SetTexture(textTexture);
	copySlot.countText.SetCharsInRow(10);
	copySlot.countText.SetCharsInColumn(10);
	copySlot.countText.SetCharsCount(10 * 10);
	copySlot.countText.SetText(textCount.c_str());
	copySlot.countText.Init();

	copySlot.description.SetWindow(mainWindow);
	copySlot.description.SetTextTexture(textTexture);
	copySlot.description.Init(desctiptionWidth, DESCRIPTION_HEIGHT);

	copySlot.block = std::move(draggingSlot.block);

	copySlot.mesh.GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	copySlot.mesh.SetRectangleUV(uvs[static_cast<int>(copySlot.block.GetBlockType())][1]);
	copySlot.mesh.Init();

	copySlot.actor.SetMesh(&copySlot.mesh);
	copySlot.actor.SetTexture(itemTexture);

	copySlot.count = draggingSlot.count;
	// ------------------------ Copy Slot ------------------------ //


	// ------------------------ Slot 1 ------------------------ //
	textCount = std::to_string(slot2.count);
	blockDescription = const_cast<char*>(slot2.block.GetBlockText());
	desctiptionWidth = strlen(blockDescription) * Text::CHAR_WIDTH / 2;

	slot1.countText.SetText(textCount.c_str());
	slot1.countText.Init();

	slot1.description.SetTextTexture(textTexture);
	slot1.description.Init(desctiptionWidth, DESCRIPTION_HEIGHT);

	slot1.block = std::move(slot2.block);

	slot1.mesh.SetRectangleUV(uvs[static_cast<int>(slot1.block.GetBlockType())][1]);
	slot1.mesh.Init();

	slot1.actor.SetMesh(&slot1.mesh);
	slot1.actor.SetTexture(itemTexture);

	slot1.count = slot2.count;
	// ------------------------ Slot 1 ------------------------ //


	// ------------------------ Slot 2 ------------------------ //
	textCount = std::to_string(copySlot.count);
	blockDescription = const_cast<char*>(copySlot.block.GetBlockText());
	desctiptionWidth = strlen(blockDescription) * Text::CHAR_WIDTH / 2;

	slot2.countText.SetText(textCount.c_str());
	slot2.countText.Init();

	slot2.description.SetTextTexture(textTexture);
	slot2.description.Init(desctiptionWidth, DESCRIPTION_HEIGHT);

	slot2.block = std::move(copySlot.block);

	slot2.mesh.SetRectangleUV(uvs[static_cast<int>(slot2.block.GetBlockType())][1]);
	slot2.mesh.Init();

	slot2.actor.SetMesh(&slot2.mesh);
	slot2.actor.SetTexture(itemTexture);

	slot2.count = copySlot.count;
	// ------------------------ Slot 2 ------------------------ //
}