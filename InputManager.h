#pragma once

#include "Framework.h"
#include "Window.h"

class InputManager
{
private:
	Window* window;

public:
	explicit InputManager(Window* window) noexcept;

	void BindMouseCallback(void(*event)(GLFWwindow* window, double x, double y));
	void EnableGamemode();

	bool GetKeyState(int key) const;
	bool IsKeyHoldForTime(int key, int milliseconds) const;

	bool GetMouseButtonState(int button) const;
	bool IsMouseButtonHoldForTime(int button, int milliseconds) const;

	GLFWwindow* GetWindow() const;

};