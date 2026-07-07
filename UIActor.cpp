#include "UIActor.h"

void UIActor::SetMesh(UIMesh* mesh) noexcept
{
	this->mesh = mesh;
}

void UIActor::SetPenSize(float size) const noexcept
{
	glLineWidth(size);
}

void UIActor::SetPosition(const glm::vec3& vector) noexcept
{
	pos = vector;
}

UIMesh* UIActor::GetMesh() noexcept
{
	return mesh;
}

glm::mat4 UIActor::GetModelMatrix() const noexcept
{
	glm::mat4 model(1.f);

	model = glm::translate(model, pos);
	return model;
}