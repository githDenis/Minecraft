#pragma once

#include "Framework.h"
#include "UIMesh.h"

class UIActor
{
private:
	UIMesh* mesh;
	glm::vec3 pos{ 0.f, 0.f, 0.f };

public:
	UIActor() noexcept {};

	void SetMesh(UIMesh* mesh) noexcept;
	void SetPenSize(float size) const noexcept;
	void SetPosition(const glm::vec3& vector) noexcept;
	UIMesh* GetMesh() noexcept;
	glm::mat4 GetModelMatrix() const noexcept;
};