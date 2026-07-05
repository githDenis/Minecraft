#pragma once
#include "Framework.h"
#include "Window.h"

class Inventory
{
private:
	Window window;
	
public:
	void InitWindow(const char* title, int width, int height);
	void Show();
	void Hide();
};