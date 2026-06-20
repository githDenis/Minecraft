#include "UIActor.h"

void UIActor::SetMesh(UIMesh* mesh) noexcept
{
	this->mesh = mesh;
}

void UIActor::SetPenSize(float size) const
{
	glLineWidth(size);
}

UIMesh* UIActor::GetMesh() noexcept
{
	return mesh;
}