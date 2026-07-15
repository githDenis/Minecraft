#pragma once

#include "Framework.h"
#include "UIMesh.h"
#include "Texture.h"

class UIActor
{
private:
	UIMesh* mesh;
	Texture* texture;
	glm::vec3 pos{ 0.f, 0.f, 0.f };

public:
	UIActor() noexcept {};

	void SetMesh(UIMesh* mesh) noexcept;
	void SetTexture(Texture* texture) noexcept;
	void SetPenSize(float size) const noexcept;
	void SetPosition(const glm::vec3& vector) noexcept;
	UIMesh* GetMesh() noexcept;
	const glm::vec3& GetPosition() const noexcept;
	Texture* GetTexture() noexcept;
	glm::mat4 GetModelMatrix() const noexcept;
};