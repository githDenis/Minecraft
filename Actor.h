#pragma once

#include "Framework.h"
#include "Mesh.h"
#include "Texture.h"
#include "Structs.h"

class Actor
{
private:
	Mesh* mesh = nullptr;
	Texture* texture = nullptr;
	glm::vec3 position = { 0, 0, 0 };
	Rotator rotation = { 0, 0, 0 };

public:
	Actor() {};

	void SetMesh(Mesh* mesh) noexcept;
	void SetPosition(const glm::vec3& vector) noexcept;
	void SetRotation(const Rotator& rotation) noexcept;
	void SetTexture(Texture* texture) noexcept;

	glm::mat4 GetModelMattrix() noexcept;
	Mesh* GetMesh() noexcept;
	unsigned int GetTexture() const noexcept;
};