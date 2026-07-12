#include "Text.h"
#include <string>

void Text::SetMainWindow(Window* window) noexcept
{
	this->window = window;
}

void Text::SetTexture(Texture* texture) noexcept
{
	this->texture = texture;
}

void Text::SetCharsInRow(int charsInRow) noexcept
{
	this->charsInRow = charsInRow;
}

void Text::SetCharsInColumn(int charsInColumn) noexcept
{
	this->charsInColumn = charsInColumn;
}

void Text::SetCharsCount(int charsCount) noexcept
{
	this->charsCount = charsCount;
}

void Text::SetText(const char* buf) noexcept
{
	strcpy(this->buf, buf);
}

void Text::SetTextFromDigits(int num) noexcept
{
	SetText(std::to_string(num).c_str());
}

void Text::SetStartPosition(const glm::vec3& pos) noexcept
{
	startPos = pos;
}

void Text::Init()
{
	uvs = std::make_unique<UV[]>(charsCount);

	for (int i = 0; i < charsCount; i++)
	{
		uvs[i] = texture->GetUV(i, charsInRow, charsInColumn);
	}
}

void Text::Draw(Render* render)
{
	int size = strlen(buf);
	glm::vec3 charPos = startPos;

	for (int i = 0; i < size; i++)
	{
		if (buf[i] != '\0')
		{
			int index = static_cast<int>(buf[i]) - 48;
			UV uv{ uvs[index] };

			meshes[i].GenerateRectangle(CHAR_WIDTH, CHAR_HEIGHT, window->GetWidth(), window->GetHeight());
			meshes[i].SetRectabgleUV(uv);
			meshes[i].Init();

			actors[i].SetMesh(&meshes[i]);
			actors[i].SetTexture(texture);
			actors[i].SetPosition(charPos);
			render->DrawUIActor(actors[i], GL_TRIANGLES);
			charPos = charPos + glm::vec3(CHAR_WIDTH / 2, 0.f, 0.f);
		}
	}
}