#pragma once
#pragma warning(disable : 4996)
#include "Framework.h"
#include "Window.h"
#include "Texture.h"
#include "UIMesh.h"
#include "UIActor.h"
#include "Render.h"
#include <array>

class Text
{
private:
	static const int BUF_SIZE = 16;
	static constexpr float CHAR_WIDTH = 0.1f;
	static constexpr float CHAR_HEIGHT = 0.13f;

	int charsInRow;
	int charsInColumn;
	int charsCount;

	Window* window;
	Texture* texture;
	char buf[BUF_SIZE + 1];
	std::unique_ptr<UV[]> uvs;

	std::array<UIMesh, BUF_SIZE> meshes;
	std::array<UIActor, BUF_SIZE> actors;

	glm::vec3 startPos;

public:
	void SetMainWindow(Window* window) noexcept;
	void SetTexture(Texture* texture) noexcept;
	void SetCharsInRow(int charsInRow) noexcept;
	void SetCharsInColumn(int charsInColumn) noexcept;
	void SetCharsCount(int charsCount) noexcept;
	void SetText(const char* buf) noexcept;
	void SetStartPosition(const glm::vec3& pos) noexcept;
	void Init();
	void Draw(Render* render);
};