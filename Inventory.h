#pragma once
#include "Framework.h"
#include "Window.h"
#include "Render.h"
#include "InputManager.h"

class Inventory
{
public:
	static constexpr float INVENTORY_WIDTH = 1.5f;
	static constexpr float INVENTORY_HEIGHT = 1.f;
	static constexpr glm::vec3 START_SLOT_POS = glm::vec3(-0.33f, 0.36f, 0.f);
	static constexpr float SLOT_WIDTH = 0.125f;
	static constexpr float SLOT_HEIGHT = 0.125f;
	static constexpr float SLOT_PADDING = 0.02f;
	static constexpr int SLOT_COUNT_IN_ROW = 9;
	static constexpr int ROW_COUNT = 6;

private:
	Window* mainWindow;
	UIMesh mesh;
	UIActor actor;

	UIMesh slotMesh;
	std::array<UIActor, SLOT_COUNT_IN_ROW * ROW_COUNT> slotActors;

	void InitInventoryWindow() noexcept;
	void GenerateSlots() noexcept;

public:
	void SetMainWindow(Window* mainWindow) noexcept;
	void Init() noexcept;
	void Show(Render* render) noexcept;
	void Hide();
};