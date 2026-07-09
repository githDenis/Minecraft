#include "Inventory.h"
#include "Structs.h"

void Inventory::SetMainWindow(Window* mainWindow) noexcept
{
	this->mainWindow = mainWindow;
}

void Inventory::Init() noexcept
{
	InitInventoryWindow();
	GenerateSlots();
}

void Inventory::InitInventoryWindow() noexcept
{
	mesh.GenerateRectangle(INVENTORY_WIDTH, INVENTORY_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	mesh.SetColor(Color(0.7f, 0.7f, 0.7f));
	mesh.Init();

	actor.SetMesh(&mesh);
	actor.SetPenSize(3.f);
}

void Inventory::GenerateSlots() noexcept
{
	for (int y = 0; y < ROW_COUNT; y++)
	{  
		for (int x = 0; x < SLOT_COUNT_IN_ROW; x++)
		{
			slotMeshes[x + y * SLOT_COUNT_IN_ROW].GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
			slotMeshes[x + y * SLOT_COUNT_IN_ROW].SetColor(Color(0.5f, 0.5f, 0.5f));
			slotMeshes[x + y * SLOT_COUNT_IN_ROW].Init();

			slotActors[x + y * SLOT_COUNT_IN_ROW].SetMesh(&slotMeshes[x + y * SLOT_COUNT_IN_ROW]);

			glm::vec3 slotPos = START_SLOT_POS +
				glm::vec3(
					SLOT_WIDTH / 2 * x + SLOT_PADDING * x,
					-SLOT_HEIGHT * y - SLOT_PADDING * y,
					0.f);
			slotActors[x + y * SLOT_COUNT_IN_ROW].SetPosition(slotPos);
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
		}
	}
}

void Inventory::Hide()
{
}

void Inventory::AddItem(DroppedBlock& droppedBlock, Texture* texture, UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT])
{
	if (currentIndex < droppedBlocks.size())
	{
		BlockType blockType = droppedBlock.GetBlockType();
		droppedBlocks[currentIndex] = std::move(droppedBlock);
		droppedBlocks[currentIndex].SetAliveState(false);

		slotMeshes[currentIndex].SetRectabgleUV(uvs[static_cast<int>(blockType)][1]);
		slotMeshes[currentIndex].GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
		slotMeshes[currentIndex].Init();

		slotActors[currentIndex].SetMesh(&slotMeshes[currentIndex]);
		slotActors[currentIndex].SetTexture(texture);

		currentIndex++;
	}
}