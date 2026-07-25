#include "UIActor.h"

void UIActor::SetPenSize(float size) noexcept
{
	penSize = size;
}

glm::mat4 UIActor::GetModelMatrix() const noexcept
{
	glm::mat4 model(1.f);

	model = glm::translate(model, pos);
	return model;
}

UIActor& UIActor::operator=(const UIActor& another) noexcept
{
	mesh = another.mesh;
	texture = another.texture;
	pos = another.pos;
	penSize = another.penSize;
	return *this;
}