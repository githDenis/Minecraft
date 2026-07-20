#include "Actor.h"

glm::mat4 Actor::GetModelMattrix() noexcept
{
	glm::mat4 model = glm::mat4(1.f);

	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.yaw), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotation.pitch), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotation.roll), glm::vec3(0, 0, 1));
	model = glm::scale(model, scale);
	return model;
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