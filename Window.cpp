#include "Window.h"

Window::Window(const char* title) noexcept
{
	Init(title);
}

void Window::FramebufferChangeSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::Init(const char* title) noexcept
{
	strcpy(this->title, title);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	width = mode->width;
	height = mode->height;

	//window = glfwCreateWindow(width, height, title, monitor, nullptr);
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwSetWindowPos(window, 0, 0);
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