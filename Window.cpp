#include "Window.h"

Window::Window(const char* title, int width, int height) noexcept
{
	Init(title, width, height);
}

void Window::FramebufferChangeSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::Init(const char* title, int width, int height) noexcept
{
	strcpy(this->title, title);
	this->width = width;
	this->height = height;

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwSetWindowPos(window, 50, 50);
	glfwSetFramebufferSizeCallback(window, &FramebufferChangeSizeCallback);
}

void Window::MakeCurrent() noexcept
{
	glfwMakeContextCurrent(window);
}

void Window::Close() noexcept
{
	glfwSetWindowShouldClose(window, true);
}

void Window::Show()
{
	glfwShowWindow(window);
}

void Window::Hide()
{
	glfwHideWindow(window);
}