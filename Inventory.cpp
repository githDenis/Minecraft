#include "Inventory.h"
#include "Structs.h"
#include "Player.h"
#include <string>

const char* Inventory::CRAFTING_TEXT = "Crafting";

void Inventory::SetMainWindow(Window* mainWindow) noexcept
{
	this->mainWindow = mainWindow;

	for (int i = 0; i < SLOTS_COUNT; i++)
	{
		slots[i].countText.SetMainWindow(mainWindow);
		slots[i].description.SetWindow(mainWindow);
	}
	craftingText.SetMainWindow(mainWindow);
}

void Inventory::Init(Texture* textTexture) noexcept
{
	InitInventoryWindow();
	GenerateSlots(textTexture);
	GenerateCraftSlots(textTexture);
	GenerateCraftResultSlot(textTexture);
	InitCurrentFrame();
	InitCraftingText(textTexture);
}

void Inventory::InitInventoryWindow() noexcept
{
	mesh.GenerateRectangle(INVENTORY_WIDTH, INVENTORY_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	mesh.SetColor(INVENTORY_COLOR);
	mesh.Init();

	actor.SetMesh(&mesh);
	actor.SetPosition(INVENTORY_POS);
	actor.SetPenSize(3.f);
}

void Inventory::GenerateSlots(Texture* textTexture) noexcept
{
	for (int y = 0; y < ROW_COUNT; y++)
	{
		for (int x = 0; x < SLOT_COUNT_IN_ROW; x++)
		{
			slots[x + y * SLOT_COUNT_IN_ROW].mesh.GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
			slots[x + y * SLOT_COUNT_IN_ROW].mesh.SetColor(SLOT_COLOR);
			slots[x + y * SLOT_COUNT_IN_ROW].mesh.Init();

			glm::vec3 slotPos = START_SLOT_POS +
				glm::vec3(
					SLOT_WIDTH / 2 * x + SLOT_PADDING * x,
					-SLOT_HEIGHT * y - SLOT_PADDING * y,
					0.f);

			if (y == ROW_COUNT - 1)
			{
				slotPos.y -= SLOT_HEIGHT / 2;
			}

			slots[x + y * SLOT_COUNT_IN_ROW].actor.SetPosition(slotPos);
			slots[x + y * SLOT_COUNT_IN_ROW].actor.SetMesh(&slots[x + y * SLOT_COUNT_IN_ROW].mesh);

			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetTexture(textTexture);
			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetCharsInRow(10);
			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetCharsInColumn(10);
			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetCharsCount(10 * 10);
			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetText("");
			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetStartPosition(slotPos);
			slots[x + y * SLOT_COUNT_IN_ROW].countText.Init();

			slots[x + y * SLOT_COUNT_IN_ROW].description.SetTextTexture(textTexture);

			slots[x + y * SLOT_COUNT_IN_ROW].count = 0;
		}
	}
}

void Inventory::GenerateCraftSlots(Texture* textTexture) noexcept
{
	for (int y = 0; y < CRAFT_SLOT_IN_ROW; y++)
	{
		for (int x = 0; x < CRAFT_SLOT_IN_COLUMN; x++)
		{
			int index = x + y * CRAFT_SLOT_IN_ROW + (SLOT_COUNT_IN_ROW * ROW_COUNT);
			slots[index].mesh.GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
			slots[index].mesh.SetColor(SLOT_COLOR);
			slots[index].mesh.Init();

			glm::vec3 slotPos = START_CRAFT_SLOT_POS +
				glm::vec3(
					SLOT_WIDTH / 2 * x + SLOT_PADDING * x,
					-SLOT_HEIGHT * y - SLOT_PADDING * y,
					0.f);

			slots[index].actor.SetPosition(slotPos);
			slots[index].actor.SetMesh(&slots[index].mesh);

			slots[index].countText.SetTexture(textTexture);
			slots[index].countText.SetCharsInRow(10);
			slots[index].countText.SetCharsInColumn(10);
			slots[index].countText.SetCharsCount(10 * 10);
			slots[index].countText.SetText("");
			slots[index].countText.SetStartPosition(slotPos);
			slots[index].countText.Init();

			slots[index].description.SetTextTexture(textTexture);

			slots[index].count = 0;
		}
	}
}

void Inventory::GenerateCraftResultSlot(Texture* textTexture) noexcept
{
	slots[CRAFT_RESULT_SLOT_INDEX].mesh.GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	slots[CRAFT_RESULT_SLOT_INDEX].mesh.SetColor(SLOT_COLOR);
	slots[CRAFT_RESULT_SLOT_INDEX].mesh.Init();

	slots[CRAFT_RESULT_SLOT_INDEX].actor.SetPosition(CRAFT_RESULT_SLOT_POS);
	slots[CRAFT_RESULT_SLOT_INDEX].actor.SetMesh(&slots[CRAFT_RESULT_SLOT_INDEX].mesh);

	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetTexture(textTexture);
	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetCharsInRow(10);
	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetCharsInColumn(10);
	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetCharsCount(10 * 10);
	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetText("");
	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetStartPosition(CRAFT_RESULT_SLOT_POS);
	slots[CRAFT_RESULT_SLOT_INDEX].countText.Init();

	slots[CRAFT_RESULT_SLOT_INDEX].description.SetTextTexture(textTexture);

	slots[CRAFT_RESULT_SLOT_INDEX].count = 0;
}

void Inventory::InitCurrentFrame() noexcept
{
	currentItemFrameMesh.GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	currentItemFrameMesh.SetColor(CURRENT_FRAME_COLOR);
	currentItemFrameMesh.Init();

	currentItemFrameActor.SetMesh(&currentItemFrameMesh);
	currentItemFrameActor.SetPosition(HOT_BAR_POS);
	currentItemFrameActor.SetPenSize(10.f);
}

void Inventory::InitDraggingSlot(Slot& slot, Texture* itemTexture, Texture* textTexture,
	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	BlockType blockType = slot.block.GetBlockType();

	draggingSlot.mesh.GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	draggingSlot.mesh.SetRectangleUV(uvs[static_cast<int>(blockType)][1]);
	draggingSlot.mesh.Init();
	draggingSlot.actor.SetMesh(&draggingSlot.mesh);
	draggingSlot.actor.SetTexture(itemTexture);

	draggingSlot.countText.SetCharsInRow(10);
	draggingSlot.countText.SetCharsInColumn(10);
	draggingSlot.countText.SetCharsCount(10 * 10);
	draggingSlot.countText.SetText("");
	draggingSlot.countText.Init();

	draggingSlot.count = slot.count;

	draggingSlot.block = std::move(slot.block);

	std::string blockDescription = draggingSlot.block.GetBlockText();
	float desctiptionWidth = blockDescription.length() * Text::CHAR_WIDTH / 2;
	float desctiptionHeight = Text::CHAR_HEIGHT;

	draggingSlot.description.SetWindow(mainWindow);
	draggingSlot.description.SetTextTexture(textTexture);
	draggingSlot.description.Init(desctiptionWidth, desctiptionHeight);
}

void Inventory::InitCraftingText(Texture* textTexture) noexcept
{
	craftingText.SetCharsInRow(10);
	craftingText.SetCharsInColumn(10);
	craftingText.SetCharsCount(10 * 10);
	craftingText.SetText(CRAFTING_TEXT);
	craftingText.SetTexture(textTexture);
	craftingText.SetStartPosition(CRAFTING_TEXT_POS);
	craftingText.Init();
}

void Inventory::ShowInventory(Render* render) noexcept
{
	render->DrawUIActor(actor, GL_TRIANGLES);

	for (int y = 0; y < ROW_COUNT; y++)
	{
		for (int x = 0; x < SLOT_COUNT_IN_ROW; x++)
		{
			if (y == ROW_COUNT - 1)
			{
				glm::vec3 pos = START_SLOT_POS + glm::vec3(
					SLOT_WIDTH / 2 * x + SLOT_PADDING * x,
					-SLOT_HEIGHT * y - SLOT_PADDING * y - SLOT_HEIGHT / 2,
					0.f);

				slots[x + y * SLOT_COUNT_IN_ROW].actor.SetPosition(pos);
				slots[x + y * SLOT_COUNT_IN_ROW].countText.SetStartPosition(pos);
			}
			render->DrawUIActor(slots[x + y * SLOT_COUNT_IN_ROW].actor, GL_TRIANGLES);
			slots[x + y * SLOT_COUNT_IN_ROW].countText.Draw(render);
		}
	}

	for (int i = CRAFT_SLOT_START_INDEX; i < SLOTS_COUNT - 1; i++)
	{
		render->DrawUIActor(slots[i].actor, GL_TRIANGLES);
		slots[i].countText.Draw(render);
	}

	render->DrawUIActor(slots[CRAFT_RESULT_SLOT_INDEX].actor, GL_TRIANGLES);
	slots[CRAFT_RESULT_SLOT_INDEX].countText.Draw(render);
	craftingText.Draw(render);
}

void Inventory::ShowHotBar(Render* render) noexcept
{
	for (int i = START_HOT_BAR_SLOT_INDEX; i < START_HOT_BAR_SLOT_INDEX + SLOT_COUNT_IN_ROW; i++)
	{
		glm::vec3 pos = HOT_BAR_POS + glm::vec3(
			SLOT_WIDTH / 2 * (i - START_HOT_BAR_SLOT_INDEX) + SLOT_PADDING * (i - START_HOT_BAR_SLOT_INDEX),
			0.f,
			0.f);

		slots[i].actor.SetPosition(pos);
		slots[i].countText.SetStartPosition(pos);

		render->DrawUIActor(slots[i].actor, GL_TRIANGLES);
		slots[i].countText.Draw(render);
	}
}

void Inventory::ShowCurrentItemFrame(Render* render) noexcept
{
	render->DrawUIActor(currentItemFrameActor, GL_LINE_LOOP);
}

void Inventory::AddItem(DroppedBlock& droppedBlock, Texture* texture,
	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	int index = 0;

	for (int i = START_HOT_BAR_SLOT_INDEX; i < SLOTS_COUNT; i++)
	{
		if (slots[i].count == 0)
		{
			index = i;
			break;
		}
		else
		{
			if (slots[i].count == MAX_ITEMS_IN_SLOT)
			{
				continue;
			}
			if (droppedBlock.GetBlockType() == slots[i].block.GetBlockType())
			{
				index = i;
				break;
			}
			else
			{
				continue;
			}
		}
	}

	BlockType blockType = droppedBlock.GetBlockType();

	for (int i = index; i < SLOTS_COUNT; i++)
	{
		if (slots[i].count == 0)
		{
			slots[i].block = std::move(droppedBlock);
			slots[i].block.SetAliveState(false);
			slots[i].mesh.SetRectangleUV(uvs[static_cast<int>(blockType)][1]);
			slots[i].mesh.Init();

			slots[i].actor.SetMesh(&slots[i].mesh);
			slots[i].actor.SetTexture(texture);

			slots[i].count++;

			std::string textCount = std::to_string(slots[i].count);
			slots[i].countText.SetText(textCount.c_str());

			std::string blockDescription = slots[i].block.GetBlockText();
			float desctiptionWidth = blockDescription.length() * Text::CHAR_WIDTH / 2;

			slots[i].description.Init(desctiptionWidth, DESCRIPTION_HEIGHT);
			break;
		}

		if (slots[i].block.GetBlockType() == blockType)
		{
			if (slots[i].count == MAX_ITEMS_IN_SLOT)
			{
				continue;
			}
			slots[i].block = std::move(droppedBlock);
			slots[i].block.SetAliveState(false);
			slots[i].count++;

			std::string text = std::to_string(slots[i].count);
			slots[i].countText.SetText(text.c_str());
			break;
		}
	}
}

void Inventory::ProcessMouseHovering(InputManager* inputManager, Render* render) noexcept
{
	glm::vec2 NDCPos = inputManager->GetMouseNDC();

	for (int i = 0; i < SLOTS_COUNT; i++)
	{
		glm::vec3 slotPos = slots[i].actor.GetPosition();

		if ((NDCPos.x >= slotPos.x - SLOT_WIDTH / 4 && NDCPos.x <= slotPos.x + SLOT_WIDTH / 4) &&
			(NDCPos.y >= slotPos.y - SLOT_HEIGHT / 2 && NDCPos.y <= slotPos.y + SLOT_HEIGHT / 2))
		{
			slots[i].description.SetText(slots[i].block.GetBlockText());
			slots[i].description.SetPosition(glm::vec3(NDCPos.x, NDCPos.y + 0.1f, 0.f));
			slots[i].description.Draw(render);
		}
	}
}

void Inventory::SelectLeftItem() noexcept
{
	if (currentItem - 1 >= START_HOT_BAR_SLOT_INDEX)
	{
		currentItem--;
		glm::vec3 currentPos = currentItemFrameActor.GetPosition();
		glm::vec3 newPos = currentPos - glm::vec3(SLOT_WIDTH / 2 + SLOT_PADDING, 0.f, 0.f);
		currentItemFrameActor.SetPosition(newPos);
	}
}

void Inventory::SelectRightItem() noexcept
{
	if (currentItem + 1 <= SLOTS_COUNT - 1)
	{
		currentItem++;
		glm::vec3 currentPos = currentItemFrameActor.GetPosition();
		glm::vec3 newPos = currentPos + glm::vec3(SLOT_WIDTH / 2 + SLOT_PADDING, 0.f, 0.f);
		currentItemFrameActor.SetPosition(newPos);
	}
}

void Inventory::DecreaseCurrentItem() noexcept
{
	slots[currentItem].count--;

	if (slots[currentItem].count <= 0)
	{
		ResetSlot(slots[currentItem]);
	}
	else
	{
		std::string text = std::to_string(slots[currentItem].count);
		slots[currentItem].countText.SetText(text.c_str());
	}
}

void Inventory::ThrowOutItemFromInventory(InputManager* inputManager, World* world, Player* player,
	Texture* texture, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	glm::vec2 NDCPos = inputManager->GetMouseNDC();

	for (int i = 0; i < SLOTS_COUNT; i++)
	{
		glm::vec3 slotPos = slots[i].actor.GetPosition();

		if ((NDCPos.x >= slotPos.x - SLOT_WIDTH / 4 && NDCPos.x <= slotPos.x + SLOT_WIDTH / 4) &&
			(NDCPos.y >= slotPos.y - SLOT_HEIGHT / 2 && NDCPos.y <= slotPos.y + SLOT_HEIGHT / 2))
		{
			if (!isItemDragging && slots[i].count > 0)
			{
				if (slots[i].count > 0)
				{
					slots[i].count--;
					std::string text = std::to_string(slots[i].count);
					slots[i].countText.SetText(text.c_str());

					ThrowOutBlockFromSlot(slots[i], world, player, texture, uvs);
				}

				if (slots[i].count <= 0)
				{
					ResetSlot(slots[i]);
				}
				return;
			}
		}
	}
}

void Inventory::ThrowOutItemFromHotbar(World* world, Player* player, Texture* texture,
	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	if (slots[currentItem].count > 0)
	{
		slots[currentItem].count--;
		std::string text = std::to_string(slots[currentItem].count);
		slots[currentItem].countText.SetText(text.c_str());

		ThrowOutBlockFromSlot(slots[currentItem], world, player, texture, uvs);
	}

	if (slots[currentItem].count <= 0)
	{
		ResetSlot(slots[currentItem]);
	}
}

void Inventory::ThrowOutBlockFromSlot(const Slot& slot, World* world, Player* player, Texture* texture,
	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	BlockType blockType = slot. block.GetBlockType();
	BlockClass blockClass = slot.block.GetBlockClass();
	DroppedBlock droppedBlock;
	glm::vec3 pos = player->GetPosition() + player->GetCamera().GetFrontMovementVector() * 4.f;
	droppedBlock.Init(uvs, texture, blockClass, blockType, pos);
	world->GetDroppedBlocksArray().Add(std::move(droppedBlock));
}

void Inventory::ResetSlot(Slot& slot) noexcept
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

void Inventory::ProcessingMouseCkick(InputManager* inputManager, Texture* itemTexture,
	Texture* textTexture, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	glm::vec2 NDCPos = inputManager->GetMouseNDC();

	for (int i = 0; i < SLOTS_COUNT; i++)
	{
		glm::vec3 slotPos = slots[i].actor.GetPosition();

		if ((NDCPos.x >= slotPos.x - SLOT_WIDTH / 4 && NDCPos.x <= slotPos.x + SLOT_WIDTH / 4) &&
			(NDCPos.y >= slotPos.y - SLOT_HEIGHT / 2 && NDCPos.y <= slotPos.y + SLOT_HEIGHT / 2))
		{
			if (!isItemDragging && slots[i].count > 0)
			{
				if (i == CRAFT_RESULT_SLOT_INDEX)
				{
					UseCraftRecouses(GetOutputItemCountFromRecipe());
				}

				isItemDragging = true;
				draggingItemIndex = i;

				InitDraggingSlot(slots[i], itemTexture, textTexture, uvs);
				ResetSlot(slots[i]);
				return;
			}
		}
	}
}

void Inventory::ProcessingMouseRelease(World* world, Player* player, Texture* texture, Texture* textTexture,
	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	if (isItemDragging)
	{
		isItemDragging = false;

		bool isItemWasPut = false;

		float x = draggingSlot.actor.GetPosition().x;
		float y = draggingSlot.actor.GetPosition().y;

		BlockType blockType = draggingSlot.block.GetBlockType();

		for (int i = 0; i < SLOTS_COUNT; i++)
		{
			glm::vec3 slotPos = slots[i].actor.GetPosition();

			if ((x >= slotPos.x - SLOT_WIDTH / 4 && x <= slotPos.x + SLOT_WIDTH / 4) &&
				(y >= slotPos.y - SLOT_HEIGHT / 2 && y <= slotPos.y + SLOT_HEIGHT / 2))
			{
				isItemWasPut = true;

				if (slots[i].count <= 0)
				{
					if (i == CRAFT_RESULT_SLOT_INDEX)
					{
						for (int i = 0; i < draggingSlot.count; i++)
						{
							ThrowOutBlockFromSlot(draggingSlot, world, player, texture, uvs);
						}
					}
					else
					{
						slots[i].count = draggingSlot.count;

						slots[i].block = std::move(draggingSlot.block);
						slots[i].block.SetAliveState(false);
						slots[i].mesh.SetRectangleUV(uvs[static_cast<int>(blockType)][1]);
						slots[i].mesh.Init();

						slots[i].actor.SetMesh(&slots[i].mesh);
						slots[i].actor.SetTexture(texture);

						std::string textCount = std::to_string(slots[i].count);
						slots[i].countText.SetText(textCount.c_str());

						std::string blockDescription = slots[i].block.GetBlockText();
						float desctiptionWidth = blockDescription.length() * Text::CHAR_WIDTH / 2;
						float desctiptionHeight = Text::CHAR_HEIGHT;

						slots[i].description.Init(desctiptionWidth, desctiptionHeight);
					}
				}
				else
				{
					if (blockType == slots[i].block.GetBlockType())
					{
						slots[i].count += draggingSlot.count;
						std::string textCount = std::to_string(slots[i].count);
						slots[i].countText.SetText(textCount.c_str());
					}
					else
					{
						SwapSlots(slots[draggingItemIndex], slots[i], texture, textTexture, uvs);
					}
				}
			}
		}

		if (!isItemWasPut)
		{
			for (int i = 0; i < draggingSlot.count; i++)
			{
				ThrowOutBlockFromSlot(draggingSlot, world, player, texture, uvs);
			}
		}
		ResetSlot(draggingSlot);
		draggingSlot.actor.SetPosition(glm::vec3(-5.f, 0.f, 0.f));
	}
}

void Inventory::UpdateDraggingItemPosition(InputManager* inputManager) noexcept
{
	if (isItemDragging)
	{
		glm::vec2 NDCPos = inputManager->GetMouseNDC();
		glm::vec3 pos(NDCPos.x, NDCPos.y, 0.f);
		draggingSlot.actor.SetPosition(pos);
	}
}

void Inventory::ShowDraggingItem(Render* render) noexcept
{
	if (isItemDragging)
	{
		render->DrawUIActor(draggingSlot.actor, GL_TRIANGLES);
	}
}

void Inventory::SwapSlots(Slot& slot1, Slot& slot2, Texture* itemTexture, Texture* textTexture,
	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	std::string textCount;
	std::string blockDescription;
	float desctiptionWidth;

	textCount = std::to_string(draggingSlot.count);
	blockDescription = draggingSlot.block.GetBlockText();
	desctiptionWidth = blockDescription.length() * Text::CHAR_WIDTH / 2;

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
	blockDescription = slot2.block.GetBlockText();
	desctiptionWidth = blockDescription.length() * Text::CHAR_WIDTH / 2;

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
	blockDescription = copySlot.block.GetBlockText();
	desctiptionWidth = blockDescription.length() * Text::CHAR_WIDTH / 2;

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

void Inventory::CheckCrafting(Texture* itemTexture, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT]) noexcept
{
	std::array<BlockType, 4> currentRecipe;

	for (int i = CRAFT_SLOT_START_INDEX; i < SLOTS_COUNT - 1; i++)
	{
		currentRecipe[i - CRAFT_SLOT_START_INDEX] = slots[i].block.GetBlockType();
	}

	bool isRecipeFound = false;
	int countCoeff = 0;

	for (int i = 0; i < recipes.size(); i++)
	{
		for (int j = 0; j < recipes[0].input.size(); j++)
		{
			if (recipes[i].input[j] == currentRecipe[j])
			{
				if (j == recipes[0].input.size() - 1)
				{
					isRecipeFound = true;
					countCoeff = recipes[i].countCoefficient;
					break;
				}
				continue;
			}
			else
			{
				break;
			}
		}

		if (isRecipeFound)
		{
			slots[CRAFT_RESULT_SLOT_INDEX].mesh.SetRectangleUV(uvs[static_cast<int>(recipes[i].outputBlock)][1]);
			slots[CRAFT_RESULT_SLOT_INDEX].mesh.Init();
			slots[CRAFT_RESULT_SLOT_INDEX].actor.SetMesh(&slots[CRAFT_RESULT_SLOT_INDEX].mesh);
			slots[CRAFT_RESULT_SLOT_INDEX].actor.SetTexture(itemTexture);

			DroppedBlock newBlock;
			newBlock.Init(uvs, itemTexture, BlockClass::BC_OPAQUE, recipes[i].outputBlock, glm::vec3(0, 0, 0));
			slots[CRAFT_RESULT_SLOT_INDEX].block = std::move(newBlock);

			slots[CRAFT_RESULT_SLOT_INDEX].count = GetOutputItemCountFromRecipe() * countCoeff;

			std::string textCount = std::to_string(slots[CRAFT_RESULT_SLOT_INDEX].count);
			slots[CRAFT_RESULT_SLOT_INDEX].countText.SetText(textCount.c_str());

			slots[CRAFT_RESULT_SLOT_INDEX].countText.SetText(textCount.c_str());
			slots[CRAFT_RESULT_SLOT_INDEX].countText.Init();
			return;
		}
		else
		{
			ResetSlot(slots[CRAFT_RESULT_SLOT_INDEX]);
		}
	}
}

int Inventory::GetOutputItemCountFromRecipe() noexcept
{
	int max1 = std::max(slots[36].count, slots[37].count);
	int max2 = std::max(slots[38].count, slots[39].count);
	int maxQuantity = std::max(max1, max2);

	int counts[] = { slots[36].count, slots[37].count, slots[38].count, slots[39].count };

	int result = 0;

	for (int i = 0; i < maxQuantity; i++)
	{
		for (int j = CRAFT_SLOT_START_INDEX; j < SLOTS_COUNT - 1; j++)
		{
			if (slots[j].block.GetBlockType() != BlockType::BT_AIR)
			{
				counts[j - CRAFT_SLOT_START_INDEX]--;
			}
		}

		if (counts[0] >= 0 && counts[1] >= 0 && counts[2] >= 0 && counts[3] >= 0)
		{
			result++;
		}
	}
	return result;
}

void Inventory::UseCraftRecouses(int count) noexcept
{
	for (int i = 0; i < count; i++)
	{
		for (int j = CRAFT_SLOT_START_INDEX; j < SLOTS_COUNT - 1; j++)
		{
			if (slots[j].block.GetBlockType() != BlockType::BT_AIR)
			{
				slots[j].count--;

				if (slots[j].count <= 0)
				{
					ResetSlot(slots[j]);
				}
			}
		}
	}
}