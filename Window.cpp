#include "Window.h"

Window::Window(const char* title, int width, int height) noexcept
{
	strcpy(this->title, title);
	this->width = width;
	this->height = height;

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwSetWindowPos(window, 50, 50);
	glfwSetFramebufferSizeCallback(window, &FramebufferChangeSizeCallback);
}

void Window::FramebufferChangeSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* Window::GetHandle() const noexcept
{
	return window;
}

int Window::GetWidth() const noexcept
{
	return width;
}

int Window::GetHeight() const noexcept
{
	return height;
}