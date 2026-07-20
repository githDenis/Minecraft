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
public:
	static const int BUF_SIZE = 20;
	static constexpr float CHAR_WIDTH = 0.1f;
	static constexpr float CHAR_HEIGHT = 0.13f;

private:
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
	void SetMainWindow(Window* window) noexcept
	{
		this->window = window;
	}

	void SetTexture(Texture* texture) noexcept
	{
		this->texture = texture;
	}

	void SetCharsInRow(int charsInRow) noexcept
	{
		this->charsInRow = charsInRow;
	}

	void SetCharsInColumn(int charsInColumn) noexcept
	{
		this->charsInColumn = charsInColumn;
	}

	void SetCharsCount(int charsCount) noexcept
	{
		this->charsCount = charsCount;
	}

	void SetText(const char* buf) noexcept
	{
		strcpy(this->buf, buf);
	}

	void SetStartPosition(const glm::vec3& pos) noexcept
	{
		startPos = pos;
	}

	void Init();
	void Draw(Render* render);
};