#include "Inventory.h"
#include "Structs.h"
#include <string>

void Inventory::SetMainWindow(Window* mainWindow) noexcept
{
	this->mainWindow = mainWindow;

	for (int i = 0; i < SLOTS_COUNT; i++)
	{
		slots[i].countText.SetMainWindow(mainWindow);
		slots[i].description.SetWindow(mainWindow);
	}
}

void Inventory::Init(Texture* textTexture) noexcept
{
	InitInventoryWindow();
	GenerateSlots(textTexture);
	InitCurrentFrame();
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

void Inventory::InitCurrentFrame() noexcept
{
	currentItemFrameMesh.GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	currentItemFrameMesh.SetColor(CURRENT_FRAME_COLOR);
	currentItemFrameMesh.Init();

	currentItemFrameActor.SetMesh(&currentItemFrameMesh);
	currentItemFrameActor.SetPosition(HOT_BAR_POS);
	currentItemFrameActor.SetPenSize(10.f);
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
}

void Inventory::ShowHotBar(Render* render) noexcept
{
	static int currentItemsInventoryStartIndex = SLOT_COUNT_IN_ROW * (ROW_COUNT - 1);

	for (int i = currentItemsInventoryStartIndex; i < SLOTS_COUNT; i++)
	{
		glm::vec3 pos = HOT_BAR_POS + glm::vec3(
			SLOT_WIDTH / 2 * (i - currentItemsInventoryStartIndex) +  SLOT_PADDING *  (i - currentItemsInventoryStartIndex),
			0.f,
			0.f);

		slots[i].actor.SetPosition(pos);
		slots[i].countText.SetStartPosition(pos);

		render->DrawUIActor(slots[i].actor, GL_TRIANGLES);
		slots[i].countText.Draw(render);
	}
}

void Inventory::ShowCurrentItemFrame(Render* render)
{
	render->DrawUIActor(currentItemFrameActor, GL_LINE_LOOP);
}

void Inventory::AddItem(DroppedBlock& droppedBlock, Texture* texture, UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT])
{
	static int currentItemsInventoryStartIndex = SLOT_COUNT_IN_ROW * (ROW_COUNT - 1);
	int index = 0;

	for (int i = currentItemsInventoryStartIndex; i < SLOTS_COUNT; i++)
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
			slots[i].mesh.SetRectabgleUV(uvs[static_cast<int>(blockType)][1]);
			slots[i].mesh.Init();

			slots[i].actor.SetMesh(&slots[i].mesh);
			slots[i].actor.SetTexture(texture);

			slots[i].count++;

			std::string textCount = std::to_string(slots[i].count);
			slots[i].countText.SetText(textCount.c_str());
			
			std::string blockDescription = slots[i].block.GetBlockText();
			float desctiptionWidth = blockDescription.length() * Text::CHAR_WIDTH / 2;
			float desctiptionHeight = Text::CHAR_HEIGHT;

			slots[i].description.Init(desctiptionWidth, desctiptionHeight);
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

void Inventory::ProcessMouseHovering(InputManager* inputManager, Render* render)
{
	glm::vec2 mousePos = inputManager->GetMousePosition();

	float x = (mousePos.x / mainWindow->GetWidth()) * 2.0f - 1.0f;
	float y = 1.0f - (mousePos.y / mainWindow->GetHeight()) * 2.0f;

	glm::vec2 NDCPos = glm::vec2(x, y);

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

void Inventory::SelectLeftItem()
{
	if (currentItem - 1 >= SLOT_COUNT_IN_ROW * (ROW_COUNT - 1))
	{
		currentItem--;
		glm::vec3 currentPos = currentItemFrameActor.GetPosition();
		glm::vec3 newPos = currentPos - glm::vec3(SLOT_WIDTH / 2 + SLOT_PADDING, 0.f, 0.f);
		currentItemFrameActor.SetPosition(newPos);
	}
}

void Inventory::SelectRightItem()
{
	if (currentItem + 1 <= SLOTS_COUNT - 1)
	{
		currentItem++;
		glm::vec3 currentPos = currentItemFrameActor.GetPosition();
		glm::vec3 newPos = currentPos + glm::vec3(SLOT_WIDTH / 2 + SLOT_PADDING, 0.f, 0.f);
		currentItemFrameActor.SetPosition(newPos);
	}
}

void Inventory::DecreaseCurrentItem()
{
	slots[currentItem].count--;

	if (slots[currentItem].count <= 0)
	{
		slots[currentItem].countText.SetText("");
		slots[currentItem].description.SetText("");
		slots[currentItem].mesh.GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
		slots[currentItem].mesh.SetColor(SLOT_COLOR);
		slots[currentItem].mesh.Init();
		slots[currentItem].actor.SetMesh(&slots[currentItem].mesh);
		slots[currentItem].block = DroppedBlock();
	}
	else
	{
		std::string text = std::to_string(slots[currentItem].count);
		slots[currentItem].countText.SetText(text.c_str());
	}
}

void Inventory::ResetSlot()
{

}
