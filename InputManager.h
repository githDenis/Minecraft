#pragma once

#include "Framework.h"
#include "Window.h"
#include <array>

class InputManager
{
private:
	Window* window;
	bool keysOldStates[GLFW_KEY_LAST + 1];
	bool mouseButtonsOldStates[GLFW_MOUSE_BUTTON_LAST + 1];

public:
	explicit InputManager(Window* window) noexcept;

	void BindMouseCallback(void(*event)(GLFWwindow* window, double x, double y));
	void EnableGamemode() noexcept;
	void EnableUIMode() noexcept;

	bool IsKeyDown(int key) noexcept;
	bool IsKeyPressed(int key) noexcept;
	bool IsKeyReleased(int key) noexcept;

	bool IsMouseButtonDown(int button) noexcept;
	bool IsMouseButtonPressed(int button) noexcept;
	bool IsMouseButtonReleased(int button) noexcept;

	bool IsKeyHoldForTime(int key, int milliseconds) noexcept;
	bool IsMouseButtonHoldForTime(int button, int milliseconds) noexcept;

	GLFWwindow* GetWindow() const noexcept;

};