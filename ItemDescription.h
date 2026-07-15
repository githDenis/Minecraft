#pragma once
#include "Framework.h"
#include "UIMesh.h"
#include "UIActor.h"
#include "Text.h"

class ItemDescription
{
public:
	static constexpr Color DESCRIPTION_COLOR = Color(32.f / 255.f, 12.f / 255.f, 102.f / 255.f);

private:
	float width;
	Window* window;
	Texture* textTexture;
	UIMesh mesh;
	UIActor actor;
	Text text;

public:
	void SetWindow(Window* window) noexcept;
	void SetTextTexture(Texture* textTexture) noexcept;
	void Init(float& width, float& height) noexcept;
	void SetPosition(const glm::vec3& pos) noexcept;
	void SetText(const char* text) noexcept;
	void Draw(Render* render) noexcept;
};