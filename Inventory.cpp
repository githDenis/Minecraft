#include "Inventory.h"

void Inventory::InitWindow(const char* title, int width, int height)
{
	window.Init(title, width, height);
	window.Hide();
}

void Inventory::Show()
{
	window.Show();
}

void Inventory::Hide()
{
	window.Hide();
}