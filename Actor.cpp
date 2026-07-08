#include "Actor.h"

void Actor::SetMesh(Mesh* mesh) noexcept
{
	this->mesh = mesh;
}

void Actor::SetPosition(const glm::vec3& vector) noexcept
{
	position = vector;
}

void Actor::SetRotation(const Rotator& rotator) noexcept
{
	rotation.yaw = rotator.yaw;
	rotation.pitch = rotator.pitch;
	rotation.roll = rotator.roll;
}

void Actor::SetTexture(Texture* texture) noexcept
{
	this->texture = texture;
}

glm::mat4 Actor::GetModelMattrix() noexcept
{
	glm::mat4 model = glm::mat4(1.f);

	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.yaw), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotation.pitch), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotation.roll), glm::vec3(0, 0, 1));
	return model;
}

const glm::vec3& Actor::GetPosition() const noexcept
{
	return position;
}

const Rotator& Actor::GetRotation() const noexcept
{
	return rotation;
}

Mesh* Actor::GetMesh() noexcept
{
	return mesh;
}

unsigned int Actor::GetTexture() const noexcept
{
	return texture->GetID();
}

Actor& Actor::operator=(Actor&& another) noexcept
{
	mesh = another.mesh;
	texture = another.texture;
	position = another.position;
	rotation = another.rotation;
	another.mesh = nullptr;
	another.texture = nullptr;
	return *this;
}