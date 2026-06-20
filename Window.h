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
	Window(const char* title, int width, int height) noexcept;

	GLFWwindow* GetHandle() const noexcept;
	int GetWidth() const noexcept;
	int GetHeight() const noexcept;
};