#include "InputManager.h"

InputManager::InputManager(Window* window) noexcept
{
	this->window = window;
}

void InputManager::BindMouseCallback(void(*event)(GLFWwindow* window, double x, double y))
{
	glfwSetCursorPosCallback(window->GetHandle(), event);
}

void InputManager::EnableGamemode()
{
	glfwSetInputMode(window->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool InputManager::GetKeyState(int key) const
{
	return glfwGetKey(window->GetHandle(), key) == GLFW_PRESS ? true : false;
}

bool InputManager::IsKeyHoldForTime(int key, int time) const
{
	static bool isButtonPressed = false;
	static std::chrono::time_point<std::chrono::steady_clock> start;
	static std::chrono::time_point<std::chrono::steady_clock> end;
	static std::chrono::seconds duration = std::chrono::seconds(0);

	if (glfwGetKey(window->GetHandle(), key) == GLFW_PRESS)
	{
		if (!isButtonPressed)
		{
			isButtonPressed = true;
			start = std::chrono::steady_clock::now();
		}
	}
	else
	{
		isButtonPressed = false;
	}

	if (isButtonPressed)
	{
		end = std::chrono::steady_clock::now();
		duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	}
	else
	{
		duration = std::chrono::seconds(0);
	}
	return duration.count() >= time;
}

GLFWwindow* InputManager::GetWindow() const
{
	return window->GetHandle();
}