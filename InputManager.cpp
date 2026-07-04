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
	static bool oldState = false;
	bool res = glfwGetKey(window->GetHandle(), key) == GLFW_PRESS;

	if (!oldState && res)
	{
		oldState = true;
		return true;
	}
	else if (oldState && !res)
	{
		oldState = false;
		return false;
	}
	return false;
}

bool InputManager::IsKeyHoldForTime(int key, int milliseconds) const
{
	static bool isButtonPressed = false;
	static std::chrono::time_point<std::chrono::steady_clock> start;
	static std::chrono::time_point<std::chrono::steady_clock> end;
	static std::chrono::milliseconds duration = std::chrono::milliseconds(0);

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
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	}
	else
	{
		duration = std::chrono::milliseconds(0);
	}
	return duration.count() >= milliseconds;
}

bool InputManager::GetMouseButtonState(int button) const
{
	static bool oldState = false;
	bool res = glfwGetMouseButton(window->GetHandle(), button) == GLFW_PRESS;

	if (!oldState && res)
	{
		oldState = true;
		return true;
	}
	else if (oldState && !res)
	{
		oldState = false;
		return false;
	}
	return false;
}

bool InputManager::IsMouseButtonHoldForTime(int button, int milliseconds) const
{
	static bool isButtonPressed = false;
	static std::chrono::time_point<std::chrono::steady_clock> start;
	static std::chrono::time_point<std::chrono::steady_clock> end;
	static std::chrono::milliseconds duration = std::chrono::milliseconds(0);

	if (glfwGetMouseButton(window->GetHandle(), button) == GLFW_PRESS)
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
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	}
	else
	{
		duration.zero();
	}

	bool res = duration.count() >= milliseconds;

	if (res)
	{
		start = std::chrono::steady_clock::now();
		duration.zero();
	}
	return res;
}

GLFWwindow* InputManager::GetWindow() const
{
	return window->GetHandle();
}