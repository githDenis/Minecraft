#include "UIActor.h"

void UIActor::SetPenSize(float size) const noexcept
{
	glLineWidth(size);
}

glm::mat4 UIActor::GetModelMatrix() const noexcept
{
	glm::mat4 model(1.f);

	model = glm::translate(model, pos);
	return model;
}