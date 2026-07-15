#include "Inventory.h"
#include "Structs.h"
#include <string>

void Inventory::SetMainWindow(Window* mainWindow) noexcept
{
	this->mainWindow = mainWindow;

	for (int i = 0; i < SLOTS_COUNT; i++)
	{
		texts[i].SetMainWindow(mainWindow);
		itemDescriptions[i].SetWindow(mainWindow);
	}
}

void Inventory::Init(Texture* textTexture) noexcept
{
	InitInventoryWindow();
	GenerateSlots(textTexture);
}

void Inventory::InitInventoryWindow() noexcept
{
	mesh.GenerateRectangle(INVENTORY_WIDTH, INVENTORY_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	mesh.SetColor(Color(0.7f, 0.7f, 0.7f));
	mesh.Init();

	actor.SetMesh(&mesh);
	actor.SetPenSize(3.f);
}

void Inventory::GenerateSlots(Texture* textTexture) noexcept
{
	for (int y = 0; y < ROW_COUNT; y++)
	{
		for (int x = 0; x < SLOT_COUNT_IN_ROW; x++)
		{
			slotMeshes[x + y * SLOT_COUNT_IN_ROW].GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
			slotMeshes[x + y * SLOT_COUNT_IN_ROW].SetColor(SLOT_COLOR);
			slotMeshes[x + y * SLOT_COUNT_IN_ROW].Init();

			glm::vec3 slotPos = START_SLOT_POS +
				glm::vec3(
					SLOT_WIDTH / 2 * x + SLOT_PADDING * x,
					-SLOT_HEIGHT * y - SLOT_PADDING * y,
					0.f);
			slotActors[x + y * SLOT_COUNT_IN_ROW].SetPosition(slotPos);
			slotActors[x + y * SLOT_COUNT_IN_ROW].SetMesh(&slotMeshes[x + y * SLOT_COUNT_IN_ROW]);

			texts[x + y * SLOT_COUNT_IN_ROW].SetTexture(textTexture);
			texts[x + y * SLOT_COUNT_IN_ROW].SetCharsInRow(10);
			texts[x + y * SLOT_COUNT_IN_ROW].SetCharsInColumn(10);
			texts[x + y * SLOT_COUNT_IN_ROW].SetCharsCount(10 * 10);
			texts[x + y * SLOT_COUNT_IN_ROW].SetText("");
			texts[x + y * SLOT_COUNT_IN_ROW].SetStartPosition(slotPos + glm::vec3(0.f, 0.f, 0.f));
			texts[x + y * SLOT_COUNT_IN_ROW].Init();

			itemDescriptions[x + y * SLOT_COUNT_IN_ROW].SetTextTexture(textTexture);

			itemsCount[x + y * SLOT_COUNT_IN_ROW] = 0;
		}
	}
}

void Inventory::Show(Render* render) noexcept
{
	render->DrawUIActor(actor, GL_TRIANGLES);

	for (int y = 0; y < ROW_COUNT; y++)
	{
		for (int x = 0; x < SLOT_COUNT_IN_ROW; x++)
		{
			render->DrawUIActor(slotActors[x + y * SLOT_COUNT_IN_ROW], GL_TRIANGLES);
			texts[x + y * SLOT_COUNT_IN_ROW].Draw(render);
		}
	}
}

void Inventory::AddItem(DroppedBlock& droppedBlock, Texture* texture, UV uvs[Chunck::BLOCKS_COUNT][Chunck::UVS_COUNT])
{
	for (int i = 0; i < droppedBlocks.size(); i++)
	{
		BlockType blockType = droppedBlock.GetBlockType();

		if (itemsCount[i] == 0)
		{
			droppedBlocks[i] = std::move(droppedBlock);
			droppedBlocks[i].SetAliveState(false);
			slotMeshes[i].SetRectabgleUV(uvs[static_cast<int>(blockType)][1]);
			slotMeshes[i].GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
			slotMeshes[i].Init();

			slotActors[i].SetMesh(&slotMeshes[i]);
			slotActors[i].SetTexture(texture);

			itemsCount[i]++;

			std::string textCount = std::to_string(itemsCount[i]);
			texts[i].SetText(textCount.c_str());
			
			std::string blockDescription = droppedBlocks[i].GetBlockText();
			float desctiptionWidth = blockDescription.length() * Text::CHAR_WIDTH / 2;
			float desctiptionHeight = Text::CHAR_HEIGHT;

			itemDescriptions[i].Init(desctiptionWidth, desctiptionHeight);
			break;
		}
		
		if (droppedBlocks[i].GetBlockType() == blockType)
		{
			if (itemsCount[i] == MAX_ITEMS_IN_SLOT)
			{
				continue;
			}
			droppedBlocks[i] = std::move(droppedBlock);
			droppedBlocks[i].SetAliveState(false);
			itemsCount[i]++;

			std::string text = std::to_string(itemsCount[i]);
			texts[i].SetText(text.c_str());
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
		glm::vec3 slotPos = slotActors[i].GetPosition();

		if ((NDCPos.x >= slotPos.x - SLOT_WIDTH / 2 + SLOT_PADDING * 2 && NDCPos.x <= slotPos.x + SLOT_WIDTH / 2 - SLOT_PADDING * 2) &&
			(NDCPos.y >= slotPos.y - SLOT_HEIGHT / 2 && NDCPos.y <= slotPos.y + SLOT_HEIGHT / 2))
		{
			itemDescriptions[i].SetText(droppedBlocks[i].GetBlockText());
			itemDescriptions[i].SetPosition(glm::vec3(NDCPos.x, NDCPos.y, 0.f));
			itemDescriptions[i].Draw(render);
		}
	}
}