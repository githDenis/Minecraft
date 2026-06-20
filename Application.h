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
	Camera* camera;
	ShaderProgram* shaderProgram;
	ShaderProgram* UIShaderProgram;
	World world;
	Player* player;

public:
	~Application();

	void Init();
	void InitOpenGLContext();
	void SetWindow(Window* window) noexcept;
	void SetInputManager(InputManager* inputManager) noexcept;
	void SetRender(Render* render) noexcept;
	void SetCamera(Camera* camera) noexcept;
	void SetShaderProgram(ShaderProgram* shaderProgram) noexcept;
	void SetUIShaderProgram(ShaderProgram* shaderProgram) noexcept;
	void SetPlayer(Player* player) noexcept;
	void Run();
};