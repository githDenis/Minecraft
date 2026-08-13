#include "CraftingTableInventory.h"

const char* CraftingTableInventory::CRAFTING_TEXT = "Crafring";

void CraftingTableInventory::InitWindow()
{
	for (int i = 0; i < SLOTS_COUNT; i++)
	{
		slots[i].countText.SetMainWindow(mainWindow);
		slots[i].description.SetWindow(mainWindow);
	}
	craftingText.SetMainWindow(mainWindow);

	actor.GetMesh().GenerateRectangle(width, height, mainWindow->GetWidth(), mainWindow->GetHeight());
	actor.GetMesh().SetColor(INVENTORY_COLOR);
	actor.GetMesh().Init();
	actor.SetPosition(INVENTORY_POS);
	actor.SetPenSize(3.f);
}

void CraftingTableInventory::InitUI()
{
	InitCraftingText(textTexture);
	GenerateSlots(textTexture);
	GenerateCraftSlots(textTexture);
	GenerateCraftResultSlot(textTexture);
}

void CraftingTableInventory::Draw(Render* render)
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

void CraftingTableInventory::ProcessMouseCkick(InputManager* inputManager, Texture* itemTexture, Texture* textTexture,
	BlockUVs& uvs) noexcept
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

void CraftingTableInventory::ProcessMouseRelease(World* world, Texture* texture, Texture* textTexture,
	BlockUVs& uvs) noexcept
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
							//ThrowOutBlockFromSlot(draggingSlot, world, texture, uvs);
						}
					}
					else
					{
						slots[i].count = draggingSlot.count;

						slots[i].block = std::move(draggingSlot.block);
						slots[i].block.SetAliveState(false);
						slots[i].actor.GetMesh().SetRectangleUV(uvs[static_cast<int>(blockType)][1]);
						slots[i].actor.GetMesh().Init();
						slots[i].actor.SetTexture(texture);
						slots[i].countText.SetText(slots[i].count);

						const char* blockDescription = slots[i].block.GetBlockText();
						float desctiptionWidth = strlen(blockDescription) * Text::CHAR_WIDTH / 2;

						slots[i].description.Init(desctiptionWidth, DESCRIPTION_HEIGHT);
					}
				}
				else
				{
					if (blockType == slots[i].block.GetBlockType())
					{
						slots[i].count += draggingSlot.count;
						slots[i].countText.SetText(slots[i].count);
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
				//ThrowOutBlockFromSlot(draggingSlot, world, player, texture, uvs);
			}
		}
		ResetSlot(draggingSlot);
		draggingSlot.actor.SetPosition(glm::vec3(-5.f, 0.f, 0.f));
	}
}

void CraftingTableInventory::ProcessMouseHovering(InputManager* inputManager, Render* render) noexcept
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

void CraftingTableInventory::SplitItems(Texture* itemTexture, BlockUVs& uvs) noexcept
{
	float x = draggingSlot.actor.GetPosition().x;
	float y = draggingSlot.actor.GetPosition().y;

	BlockType blockType = draggingSlot.block.GetBlockType();

	for (int i = 0; i < SLOTS_COUNT; i++)
	{
		glm::vec3 slotPos = slots[i].actor.GetPosition();

		if ((x >= slotPos.x - SLOT_WIDTH / 4 && x <= slotPos.x + SLOT_WIDTH / 4) &&
			(y >= slotPos.y - SLOT_HEIGHT / 2 && y <= slotPos.y + SLOT_HEIGHT / 2))
		{
			if (draggingSlot.count > 0)
			{
				draggingSlot.count--;

				slots[i].count++;

				BlockRenderClass blockRenderClass = draggingSlot.block.GetBlockRenderClass();
				BlockType blockType = draggingSlot.block.GetBlockType();

				DroppedBlock block;
				block.Init(uvs, itemTexture, blockRenderClass, blockType, glm::vec3(0.f, 0.f, 0.f));

				slots[i].block = std::move(block);
				slots[i].block.SetAliveState(false);
				slots[i].actor.GetMesh().SetRectangleUV(uvs[static_cast<int>(blockType)][1]);
				slots[i].actor.GetMesh().Init();
				slots[i].actor.SetTexture(itemTexture);
				slots[i].countText.SetText(slots[i].count);

				const char* blockDescription = copySlot.block.GetBlockText();
				float desctiptionWidth = strlen(blockDescription) * Text::CHAR_WIDTH / 2;

				slots[i].description.Init(desctiptionWidth, DESCRIPTION_HEIGHT);

				if (draggingSlot.count <= 0)
				{
					ResetSlot(draggingSlot);
					isItemDragging = false;
					return;
				}
			}
		}
	}
}

Slot& CraftingTableInventory::GetSlotRefByIndex(int index) noexcept
{
	return slots[index];
}

const Slot& CraftingTableInventory::GetSlotCopyByIndex(int index) const noexcept
{
	return slots[index];
}

void CraftingTableInventory::GenerateSlots(Texture* textTexture) noexcept
{
	for (int y = 0; y < ROW_COUNT; y++)
	{
		for (int x = 0; x < SLOT_COUNT_IN_ROW; x++)
		{
			slots[x + y * SLOT_COUNT_IN_ROW].actor.GetMesh().GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
			slots[x + y * SLOT_COUNT_IN_ROW].actor.GetMesh().SetColor(SLOT_COLOR);
			slots[x + y * SLOT_COUNT_IN_ROW].actor.GetMesh().Init();

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

			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetTexture(textTexture);
			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetCharsInRow(10);
			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetCharsInColumn(10);
			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetCharsCount(10 * 10);
			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetText("");
			slots[x + y * SLOT_COUNT_IN_ROW].countText.SetStartPosition(slotPos);
			slots[x + y * SLOT_COUNT_IN_ROW].countText.Init();

			slots[x + y * SLOT_COUNT_IN_ROW].description.SetTextTexture(textTexture);
			slots[x + y * SLOT_COUNT_IN_ROW].description.InitText();

			slots[x + y * SLOT_COUNT_IN_ROW].count = 0;
		}
	}
}

void CraftingTableInventory::GenerateCraftSlots(Texture* textTexture) noexcept
{
	for (int y = 0; y < CRAFT_SLOT_IN_ROW; y++)
	{
		for (int x = 0; x < CRAFT_SLOT_IN_COLUMN; x++)
		{
			int index = x + y * CRAFT_SLOT_IN_ROW + (SLOT_COUNT_IN_ROW * ROW_COUNT);
			slots[index].actor.GetMesh().GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
			slots[index].actor.GetMesh().SetColor(SLOT_COLOR);
			slots[index].actor.GetMesh().Init();

			glm::vec3 slotPos = START_CRAFT_SLOT_POS +
				glm::vec3(
					SLOT_WIDTH / 2 * x + SLOT_PADDING * x,
					-SLOT_HEIGHT * y - SLOT_PADDING * y,
					0.f);

			slots[index].actor.SetPosition(slotPos);

			slots[index].countText.SetTexture(textTexture);
			slots[index].countText.SetCharsInRow(10);
			slots[index].countText.SetCharsInColumn(10);
			slots[index].countText.SetCharsCount(10 * 10);
			slots[index].countText.SetText("");
			slots[index].countText.SetStartPosition(slotPos);
			slots[index].countText.Init();

			slots[index].description.SetTextTexture(textTexture);
			slots[index].description.InitText();

			slots[index].count = 0;
		}
	}
}

void CraftingTableInventory::GenerateCraftResultSlot(Texture* textTexture) noexcept
{
	slots[CRAFT_RESULT_SLOT_INDEX].actor.GetMesh().GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	slots[CRAFT_RESULT_SLOT_INDEX].actor.GetMesh().SetColor(SLOT_COLOR);
	slots[CRAFT_RESULT_SLOT_INDEX].actor.GetMesh().Init();
	slots[CRAFT_RESULT_SLOT_INDEX].actor.SetPosition(CRAFT_RESULT_SLOT_POS);

	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetTexture(textTexture);
	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetCharsInRow(10);
	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetCharsInColumn(10);
	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetCharsCount(10 * 10);
	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetText("");
	slots[CRAFT_RESULT_SLOT_INDEX].countText.SetStartPosition(CRAFT_RESULT_SLOT_POS);
	slots[CRAFT_RESULT_SLOT_INDEX].countText.Init();

	slots[CRAFT_RESULT_SLOT_INDEX].description.SetTextTexture(textTexture);
	slots[CRAFT_RESULT_SLOT_INDEX].description.InitText();

	slots[CRAFT_RESULT_SLOT_INDEX].count = 0;
}

void CraftingTableInventory::InitDraggingSlot(Slot& slot, Texture* itemTexture, Texture* textTexture, 
	BlockUVs& uvs) noexcept
{
	BlockType blockType = slot.block.GetBlockType();

	draggingSlot.actor.GetMesh().GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	draggingSlot.actor.GetMesh().SetRectangleUV(uvs[static_cast<int>(blockType)][1]);
	draggingSlot.actor.GetMesh().Init();
	draggingSlot.actor.SetTexture(itemTexture);

	draggingSlot.countText.SetCharsInRow(10);
	draggingSlot.countText.SetCharsInColumn(10);
	draggingSlot.countText.SetCharsCount(10 * 10);
	draggingSlot.countText.SetText("");
	draggingSlot.countText.Init();

	draggingSlot.count = slot.count;

	draggingSlot.block = std::move(slot.block);

	const char* blockDescription = draggingSlot.block.GetBlockText();
	float desctiptionWidth = strlen(blockDescription) * Text::CHAR_WIDTH / 2;

	draggingSlot.description.SetWindow(mainWindow);
	draggingSlot.description.SetTextTexture(textTexture);
	draggingSlot.description.InitText();
	draggingSlot.description.Init(desctiptionWidth, DESCRIPTION_HEIGHT);
}

void CraftingTableInventory::InitCraftingText(Texture* textTexture) noexcept
{
	craftingText.SetCharsInRow(10);
	craftingText.SetCharsInColumn(10);
	craftingText.SetCharsCount(10 * 10);
	craftingText.SetText(CRAFTING_TEXT);
	craftingText.SetTexture(textTexture);
	craftingText.SetStartPosition(CRAFTING_TEXT_POS);
	craftingText.Init();
}

void CraftingTableInventory::CheckCrafting(World* world, Texture* itemTexture, BlockUVs& uvs) noexcept
{
	std::array<BlockType, 9> currentRecipe3x3;
	
	for (int i = CRAFT_SLOT_START_INDEX; i < SLOTS_COUNT - 1; i++)
	{
		currentRecipe3x3[i - CRAFT_SLOT_START_INDEX] = slots[i].block.GetBlockType();
	}

	bool isRecipeFound = craftingSystem->FindRecipe(currentRecipe3x3);
	int countCoeff = craftingSystem->GetCurrentCountCoeff();
	BlockType outputBlockType = craftingSystem->GetCurrentOutputBlockType();

	if (isRecipeFound)
	{
		slots[CRAFT_RESULT_SLOT_INDEX].actor.GetMesh().SetRectangleUV(uvs[static_cast<int>(outputBlockType)][1]);
		slots[CRAFT_RESULT_SLOT_INDEX].actor.GetMesh().Init();
		slots[CRAFT_RESULT_SLOT_INDEX].actor.SetTexture(itemTexture);

		BlockRenderClass blockRenderClass = world->GetBlockRenderClassByType(outputBlockType);

		DroppedBlock newBlock;

		newBlock.Init(uvs, itemTexture, blockRenderClass, outputBlockType, glm::vec3(0, 0, 0));
		slots[CRAFT_RESULT_SLOT_INDEX].block = std::move(newBlock);

		slots[CRAFT_RESULT_SLOT_INDEX].count = GetOutputItemCountFromRecipe() * countCoeff;
		slots[CRAFT_RESULT_SLOT_INDEX].countText.SetText(slots[CRAFT_RESULT_SLOT_INDEX].count);
		slots[CRAFT_RESULT_SLOT_INDEX].countText.Init();
		return;
	}
	else
	{
		ResetSlot(slots[CRAFT_RESULT_SLOT_INDEX]);
	}
}

int CraftingTableInventory::GetOutputItemCountFromRecipe() noexcept
{
	int maxQuantity = GetMaxCountOfItemInCraftSlot();

	int counts[] = { 
		slots[36].count, slots[37].count, slots[38].count, 
		slots[39].count, slots[40].count, slots[41].count,
		slots[42].count, slots[43].count, slots[44].count,
	};

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

void CraftingTableInventory::UseCraftRecouses(int count) noexcept
{
	for (int i = 0; i < count; i++)
	{
		for (int j = CRAFT_SLOT_START_INDEX; j < SLOTS_COUNT - 1; j++)
		{
			if (slots[j].block.GetBlockType() != BlockType::BT_AIR)
			{
				slots[j].count--;
				slots[j].countText.SetText(slots[j].count);

				if (slots[j].count <= 0)
				{
					ResetSlot(slots[j]);
				}
			}
		}
	}
}

int CraftingTableInventory::GetMaxCountOfItemInCraftSlot() noexcept
{
	int maxCount = 0;

	for (int i = CRAFT_SLOT_START_INDEX; i < SLOTS_COUNT - 1; i++)
	{
		if (slots[i].count > maxCount)
		{
			maxCount = slots[i].count;
		}
	}
	return maxCount;
}