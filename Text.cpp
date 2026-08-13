#include "Text.h"

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
			int index = static_cast<int>(buf[i]) - 32;
			UV uv{ uvs[index] };

			actors[i].GetMesh().GenerateRectangle(CHAR_WIDTH, CHAR_HEIGHT, window->GetWidth(), window->GetHeight());
			actors[i].GetMesh().SetRectangleUV(uv);
			actors[i].GetMesh().Init();
			actors[i].SetTexture(texture);
			actors[i].SetPosition(charPos);
			render->DrawUIActor(actors[i], GL_TRIANGLES);
			charPos = charPos + glm::vec3(CHAR_WIDTH / 6, 0.f, 0.f);
		}
	}
}

void Text::SetText(int value) noexcept
{
	static const int SIZE = 4;
	char buf[SIZE];
	itoa(value, buf, 10);
	strcpy(this->buf, buf);
}

Text& Text::operator=(const Text& another) noexcept
{
	charsInRow = another.charsInRow;
	charsInColumn = another.charsInColumn;
	charsCount = another.charsCount;

	window = another.window;
	texture = another.texture;

	actors = another.actors;
	
	startPos = another.startPos;
	
	strcpy(const_cast<char*>(buf), another.buf);

	if (!uvs.get())
	{
		uvs = std::make_unique<UV[]>(charsCount);
	}

	for (int i = 0; i < charsCount; i++)
	{
		uvs[i] = another.uvs[i];
	}
	return *this;
}