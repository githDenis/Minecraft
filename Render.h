#pragma once

#include "Framework.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "Structs.h"
#include "Actor.h"
#include "UIActor.h"
#include "Camera.h"

class Render
{
private:
	Window* window;
	ShaderProgram* shaderProgram;
	ShaderProgram* UIShaderProgram;

public:
	Render(Window* window, ShaderProgram* shaderProgram, ShaderProgram* UIshaderProgram) noexcept;

	void Init();
	void Clear(const Color& color);
	void SetViewport();
	void DrawActor(Actor& actor);
	void DrawUIActor(UIActor& actor);
	void ApplyCameraData(Camera& camera);
};