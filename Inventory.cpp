#include "Inventory.h"
#include "Structs.h"

/*
	|1| |2| |3| |4| |5| |6| |7| |8| |9|
	|10| |11| |12| |13| |14| |15| |16| |17| |18|
*/

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
	slotMesh.GenerateRectangle(SLOT_WIDTH, SLOT_HEIGHT, mainWindow->GetWidth(), mainWindow->GetHeight());
	slotMesh.SetColor(Color(0.5f, 0.5f, 0.5f));
	slotMesh.Init();

	for (int y = 0; y < ROW_COUNT; y++)
	{
		for (int x = 0; x < SLOT_COUNT_IN_ROW; x++)
		{
			slotActors[x + y * SLOT_COUNT_IN_ROW].SetMesh(&slotMesh);
			slotActors[x + y * SLOT_COUNT_IN_ROW].SetPenSize(3.f);

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