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

			meshes[i].GenerateRectangle(CHAR_WIDTH, CHAR_HEIGHT, window->GetWidth(), window->GetHeight());
			meshes[i].SetRectabgleUV(uv);
			meshes[i].Init();

			actors[i].SetMesh(&meshes[i]);
			actors[i].SetTexture(texture);
			actors[i].SetPosition(charPos);
			render->DrawUIActor(actors[i], GL_TRIANGLES);
			charPos = charPos + glm::vec3(CHAR_WIDTH / 6, 0.f, 0.f);
		}
	}
}