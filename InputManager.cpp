#include "InputManager.h"

glm::vec2 InputManager::mousePos = glm::vec2(0.f, 0.f);

InputManager::InputManager(Window* window) noexcept
{
	this->window = window;
	memset(keysOldStates, false, sizeof(keysOldStates));
	memset(mouseButtonsOldStates, false, sizeof(mouseButtonsOldStates));
}

void InputManager::MouseMoveCallback(GLFWwindow* window, double x, double y)
{
	mousePos = glm::vec2(x, y);
}

void InputManager::BindMouseCallback()
{
	glfwSetCursorPosCallback(window->GetHandle(), InputManager::MouseMoveCallback);
}

void InputManager::EnableGamemode() noexcept
{
	glfwSetInputMode(window->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputManager::EnableUIMode() noexcept
{
	glfwSetInputMode(window->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool InputManager::IsKeyDown(int key) noexcept
{
	return glfwGetKey(window->GetHandle(), key) == GLFW_PRESS;
}

bool InputManager::IsKeyPressed(int key) noexcept
{
	bool res = glfwGetKey(window->GetHandle(), key) == GLFW_PRESS;

	if (!keysOldStates[key] && res)
	{
		keysOldStates[key] = true;
		return true;
	}
	else if (keysOldStates[key] && !res)
	{
		keysOldStates[key] = false;
		return false;
	}
	return false;
}

bool InputManager::IsKeyReleased(int key) noexcept
{
	bool res = glfwGetKey(window->GetHandle(), key) == GLFW_PRESS;

	if (keysOldStates[key] && !res)
	{
		keysOldStates[key] = false;
		return true;
	}
	if (res)
	{
		keysOldStates[key] = true;
	}
	return false;
}

bool InputManager::IsMouseButtonDown(int button) noexcept
{
	return glfwGetMouseButton(window->GetHandle(), button) == GLFW_PRESS;
}

bool InputManager::IsMouseButtonPressed(int button) noexcept
{
	bool res = glfwGetMouseButton(window->GetHandle(), button) == GLFW_PRESS;

	if (!mouseButtonsOldStates[button] && res)
	{
		mouseButtonsOldStates[button] = true;
		return true;
	}
	else if (mouseButtonsOldStates[button] && !res)
	{
		mouseButtonsOldStates[button] = false;
		return false;
	}
	return false;
}

bool InputManager::IsMouseButtonReleased(int button) noexcept
{
	bool res = glfwGetMouseButton(window->GetHandle(), button) == GLFW_PRESS;

	if (mouseButtonsOldStates[button] && !res)
	{
		mouseButtonsOldStates[button] = false;
		return true;
	}
	if (res)
	{
		mouseButtonsOldStates[button] = true;
	}
	return false;
}


bool InputManager::IsKeyHoldForTime(int key, int milliseconds) noexcept
{
	static std::chrono::time_point<std::chrono::steady_clock> start;
	static std::chrono::time_point<std::chrono::steady_clock> end;
	static std::chrono::milliseconds duration = std::chrono::milliseconds(0);

	if (glfwGetKey(window->GetHandle(), key) == GLFW_PRESS)
	{
		if (!keysOldStates[key])
		{
			keysOldStates[key] = true;
			start = std::chrono::steady_clock::now();
		}
	}
	else
	{
		keysOldStates[key] = false;
	}

	if (keysOldStates[key])
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

bool InputManager::IsMouseButtonHoldForTime(int button, int milliseconds) noexcept
{
	static std::chrono::time_point<std::chrono::steady_clock> start;
	static std::chrono::time_point<std::chrono::steady_clock> end;
	static std::chrono::milliseconds duration = std::chrono::milliseconds(0);

	if (glfwGetMouseButton(window->GetHandle(), button) == GLFW_PRESS)
	{
		if (!mouseButtonsOldStates[button])
		{
			mouseButtonsOldStates[button] = true;
			start = std::chrono::steady_clock::now();
		}
	}
	else
	{
		mouseButtonsOldStates[button] = false;
	}

	if (mouseButtonsOldStates[button])
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