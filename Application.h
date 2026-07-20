#pragma once

#include "Framework.h"
#include "Window.h"
#include "InputManager.h"
#include "Render.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "UIMesh.h"
#include "Camera.h"
#include "World.h"
#include "Player.h"

class Application
{
private:
	Window* window;
	InputManager* inputManager;
	Render* render;
	ShaderProgram* shaderProgram;
	ShaderProgram* UIShaderProgram;
	World world;
	Player* player;
	
public:
	~Application();

	void SetWindow(Window* window) noexcept
	{
		this->window = window;
	}

	void SetInputManager(InputManager* inputManager) noexcept
	{
		this->inputManager = inputManager;
	}

	void SetRender(Render* render) noexcept
	{
		this->render = render;
	}

	void SetShaderProgram(ShaderProgram* shaderProgram) noexcept
	{
		this->shaderProgram = shaderProgram;
	}

	void SetUIShaderProgram(ShaderProgram* shaderProgram) noexcept
	{
		UIShaderProgram = shaderProgram;
	}

	void SetPlayer(Player* player) noexcept
	{
		this->player = player;
	}

	void Init();
	void InitOpenGLContext();
	void Run();
};