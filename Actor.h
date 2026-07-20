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
	glm::vec3 scale = { 1.f, 1.f, 1.f };

public:
	Actor() {};

	void SetMesh(Mesh* mesh) noexcept
	{
		this->mesh = mesh;
	}

	void SetPosition(const glm::vec3& vector) noexcept
	{
		position = vector;
	}

	void SetRotation(const Rotator& rotator) noexcept
	{
		rotation.yaw = rotator.yaw;
		rotation.pitch = rotator.pitch;
		rotation.roll = rotator.roll;
	}

	void SetScale(const glm::vec3& vector) noexcept
	{
		scale = vector;
	}

	void SetTexture(Texture* texture) noexcept
	{
		this->texture = texture;
	}

	const glm::vec3& GetPosition() const noexcept
	{
		return position;
	}

	const Rotator& GetRotation() const noexcept
	{
		return rotation;
	}

	Mesh* GetMesh() noexcept
	{
		return mesh;
	}

	unsigned int GetTexture() const noexcept
	{
		return texture->GetID();
	}

	glm::mat4 GetModelMattrix() noexcept;
	Actor& operator=(Actor&& another) noexcept;
};