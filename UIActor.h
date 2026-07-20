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
	void SetMesh(UIMesh* mesh) noexcept
	{
		this->mesh = mesh;
	}

	void SetTexture(Texture* texture) noexcept
	{
		this->texture = texture;
	}

	void SetPosition(const glm::vec3& vector) noexcept
	{
		pos = vector;
	}

	UIMesh* GetMesh() noexcept
	{
		return mesh;
	}

	const glm::vec3& GetPosition() const noexcept
	{
		return pos;
	}

	Texture* GetTexture() noexcept
	{
		return texture;
	}

	UIActor() noexcept {};

	void SetPenSize(float size) const noexcept;
	glm::mat4 GetModelMatrix() const noexcept;
};