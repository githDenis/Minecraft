#include "PlayerInventory.h"
#include "Structs.h"
#include "Player.h"

const char* PlayerInventory::CRAFTING_TEXT = "Crafting";

void PlayerInventory::InitWindow()
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

void PlayerInventory::InitUI()
{
	InitCraftingText(textTexture);
	GenerateSlots(textTexture);
	GenerateCraftSlots(textTexture);
	GenerateCraftResultSlot(textTexture);
	InitCurrentFrame();
}

void PlayerInventory::Draw(Render* render)
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

void PlayerInventory::ProcessMouseCkick(InputManager* inputManager, Texture* itemTexture,
	Texture* textTexture, BlockUVs& uvs) noexcept
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

void PlayerInventory::ProcessMouseRelease(World* world, Texture* texture, Texture* textTexture,
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
							ThrowOutBlockFromSlot(draggingSlot, world, texture, uvs);
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
				ThrowOutBlockFromSlot(draggingSlot, world, texture, uvs);
			}
		}
		ResetSlot(draggingSlot);
		draggingSlot.actor.SetPosition(glm::vec3(-5.f, 0.f, 0.f));
	}
}

void PlayerInventory::ProcessMouseHovering(InputManager* inputManager, Render* render) noexcept
{
	glm::vec2 NDCPos = inputManager->GetMouseNDC();

	for (int i = 0; i < SLOTS_COUNT; i++)
	{
		glm::vec3 slotPos = slots[i].actor.GetPosition();

		if ((NDCPos.x >= slotPos.x - SLOT_WIDTH / 4 && NDCPos.x <= slotPos.x + SLOT_WIDTH / 4) &&
			(NDCPos.y >= slotPos.y - SLOT_HEIGHT / 4 && NDCPos.y <= slotPos.y + SLOT_HEIGHT))
		{
			slots[i].description.SetText(slots[i].block.GetBlockText());
			slots[i].description.SetPosition(glm::vec3(NDCPos.x, NDCPos.y + 0.1f, 0.f));
			slots[i].description.Draw(render);
		}
	}
}

Slot& PlayerInventory::GetSlotRefByIndex(int index) noexcept
{
	return slots[index];
}

const Slot& PlayerInventory::GetSlotCopyByIndex(int index) const noexcept
{
	return slots[index];
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerInventory::GenerateSlots(Texture* textTexture) noexcept
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

void PlayerInventory::GenerateCraftSlots(Texture* textTexture) noexcept
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

void PlayerInventory::GenerateCraftResultSlot(Texture* textTexture) noexcept
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

void PlayerInventory::InitCurrentFrame() noexcept
{
	currentItemFrameActor.GetMesh().GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	currentItemFrameActor.GetMesh().SetColor(CURRENT_FRAME_COLOR);
	currentItemFrameActor.GetMesh().Init();

	currentItemFrameActor.SetPosition(HOT_BAR_POS);
	currentItemFrameActor.SetPenSize(10.f);
}

void PlayerInventory::InitDraggingSlot(Slot& slot, Texture* itemTexture, Texture* textTexture, 
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

void PlayerInventory::InitCraftingText(Texture* textTexture) noexcept
{
	craftingText.SetCharsInRow(10);
	craftingText.SetCharsInColumn(10);
	craftingText.SetCharsCount(10 * 10);
	craftingText.SetText(CRAFTING_TEXT);
	craftingText.SetTexture(textTexture);
	craftingText.SetStartPosition(CRAFTING_TEXT_POS);
	craftingText.Init();
}

void PlayerInventory::ShowHotBar(Render* render) noexcept
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

void PlayerInventory::ShowCurrentItemFrame(Render* render) noexcept
{
	render->DrawUIActor(currentItemFrameActor, GL_LINE_LOOP);
}

void PlayerInventory::AddItem(DroppedBlock& droppedBlock, Texture* texture, BlockUVs& uvs) noexcept
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
			slots[i].actor.GetMesh().SetRectangleUV(uvs[static_cast<int>(blockType)][1]);
			slots[i].actor.GetMesh().Init();

			slots[i].actor.SetTexture(texture);

			slots[i].count++;

			slots[i].countText.SetText(slots[i].count);

			const char* blockDescription = slots[i].block.GetBlockText();
			float desctiptionWidth = strlen(blockDescription) * Text::CHAR_WIDTH / 2;

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
			slots[i].countText.SetText(slots[i].count);
			break;
		}
	}
}

void PlayerInventory::SelectLeftItem() noexcept
{
	if (currentItem - 1 >= START_HOT_BAR_SLOT_INDEX)
	{
		currentItem--;
		glm::vec3 currentPos = currentItemFrameActor.GetPosition();
		glm::vec3 newPos = currentPos - glm::vec3(SLOT_WIDTH / 2 + SLOT_PADDING, 0.f, 0.f);
		currentItemFrameActor.SetPosition(newPos);
	}
}

void PlayerInventory::SelectRightItem() noexcept
{
	if (currentItem + 1 <= SLOTS_COUNT - 1)
	{
		currentItem++;
		glm::vec3 currentPos = currentItemFrameActor.GetPosition();
		glm::vec3 newPos = currentPos + glm::vec3(SLOT_WIDTH / 2 + SLOT_PADDING, 0.f, 0.f);
		currentItemFrameActor.SetPosition(newPos);
	}
}

void PlayerInventory::DecreaseCurrentItem() noexcept
{
	slots[currentItem].count--;

	if (slots[currentItem].count <= 0)
	{
		ResetSlot(slots[currentItem]);
	}
	else
	{
		slots[currentItem].countText.SetText(slots[currentItem].count);
	}
}

void PlayerInventory::ThrowOutItemFromInventory(InputManager* inputManager, World* world,
	Texture* texture, BlockUVs& uvs) noexcept
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
					slots[i].countText.SetText(slots[i].count);

					ThrowOutBlockFromSlot(slots[i], world, texture, uvs);
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

void PlayerInventory::ThrowOutItemFromHotbar(World* world, Texture* texture,
	BlockUVs& uvs) noexcept
{
	if (slots[currentItem].count > 0)
	{
		slots[currentItem].count--;
		slots[currentItem].countText.SetText(slots[currentItem].count);

		ThrowOutBlockFromSlot(slots[currentItem], world, texture, uvs);
	}

	if (slots[currentItem].count <= 0)
	{
		ResetSlot(slots[currentItem]);
	}
}

void PlayerInventory::ThrowOutBlockFromSlot(const Slot& slot, World* world, Texture* texture,
	BlockUVs& uvs) noexcept
{
	BlockType blockType = slot.block.GetBlockType();
	BlockRenderClass blockRenderClass = slot.block.GetBlockRenderClass();
	DroppedBlock droppedBlock;
	glm::vec3 pos = player->GetPosition() + player->GetCamera().GetFrontMovementVector() * 4.f;
	droppedBlock.Init(uvs, texture, blockRenderClass, blockType, pos);
	world->GetDroppedBlocksArray().Add(std::move(droppedBlock));
}

void PlayerInventory::SplitItems(Texture* itemTexture, BlockUVs& uvs) noexcept
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

void PlayerInventory::CheckCrafting(World* world, Texture* itemTexture, BlockUVs& uvs) noexcept
{
	std::array<BlockType, 4> currentRecipe2x2;
	std::array<BlockType, 9> currentRecipe3x3;
	currentRecipe3x3.fill(BlockType::BT_AIR);

	for (int i = CRAFT_SLOT_START_INDEX; i < SLOTS_COUNT - 1; i++)
	{
		currentRecipe2x2[i - CRAFT_SLOT_START_INDEX] = slots[i].block.GetBlockType();
	}

	currentRecipe3x3[0] = currentRecipe2x2[0];
	currentRecipe3x3[1] = currentRecipe2x2[1];
	currentRecipe3x3[3] = currentRecipe2x2[2];
	currentRecipe3x3[4] = currentRecipe2x2[3];

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

int PlayerInventory::GetOutputItemCountFromRecipe() noexcept
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

void PlayerInventory::UseCraftRecouses(int count) noexcept
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