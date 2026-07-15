#include "ItemDescription.h"

void ItemDescription::SetWindow(Window* window) noexcept
{
	this->window = window;
	text.SetMainWindow(window);
}

void ItemDescription::SetTextTexture(Texture* textTexture) noexcept
{
	this->textTexture = textTexture;
	text.SetTexture(textTexture);
}

void ItemDescription::Init(float& width, float& height) noexcept
{
	this->width = width;

	mesh.GenerateRectangle(width, height, window->GetWidth(), window->GetHeight());
	mesh.SetColor(DESCRIPTION_COLOR);
	mesh.Init();

	actor.SetMesh(&mesh);
	actor.SetPosition(glm::vec3(0.f, 0.f, 0.f));

	text.SetCharsInRow(10);
	text.SetCharsInColumn(10);
	text.SetCharsCount(10 * 10);
	text.SetText("");
	text.Init();
}

void ItemDescription::SetPosition(const glm::vec3& pos) noexcept
{
	actor.SetPosition(pos);
}
 
void ItemDescription::SetText(const char* text) noexcept
{
	this->text.SetText(text);

	glm::vec3 pos = actor.GetPosition();
	pos.x -= width / 8;
	this->text.SetStartPosition(pos);
}

void ItemDescription::Draw(Render* render) noexcept
{
	if (actor.GetMesh())
	{
		render->DrawUIActor(actor, GL_TRIANGLES);
		text.Draw(render);
	}
}