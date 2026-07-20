#pragma once

#include "Framework.h"

#pragma warning(disable: 4996)

class Window
{
public:
	static const int MAX_TITLE_LENGTH = 30;

private:
	GLFWwindow* window;
	char title[MAX_TITLE_LENGTH];
	int width;
	int height;

public:
	GLFWwindow* GetHandle() const noexcept
	{
		return window;
	}

	int GetWidth() const noexcept
	{
		return width;
	}

	int GetHeight() const noexcept
	{
		return height;
	}

	Window() noexcept {};
	explicit Window(const char* title, int width, int height) noexcept;
	static void FramebufferChangeSizeCallback(GLFWwindow* window, int width, int height);

	void Init(const char* title, int width, int height) noexcept;
	void MakeCurrent() noexcept;
	void Close() noexcept;
	void Show();
	void Hide();
};