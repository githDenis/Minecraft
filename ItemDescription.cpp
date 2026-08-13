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

void ItemDescription::Init(float width, float height) noexcept
{
	this->width = width;

	actor.GetMesh().GenerateRectangle(width, height, window->GetWidth(), window->GetHeight());
	actor.GetMesh().SetColor(DESCRIPTION_COLOR);
	actor.GetMesh().Init();
	actor.SetPosition(glm::vec3(0.f, 0.f, 0.f));
}

void ItemDescription::InitText() noexcept
{
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
	render->DrawUIActor(actor, GL_TRIANGLES);
	text.Draw(render);
}

ItemDescription& ItemDescription::operator=(const ItemDescription& another) noexcept
{
	width = another.width;
	window = another.window;
	textTexture = another.textTexture;
	actor = another.actor;
	actor.GetMesh().SetUseTextureState(false);
	text = another.text;
	return *this;
}