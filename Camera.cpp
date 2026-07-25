#include "Camera.h"

void Camera::Update()
{
	glm::vec2 mousePos = inputManager->GetMousePosition();
	float x = mousePos.x;
	float y = mousePos.y;

	if (!isLocked)
	{
		float xOffset = x - xLast;
		float yOffset = yLast - y;
		xLast = x;
		yLast = y;

		xOffset *= SENSITIVITY;
		yOffset *= SENSITIVITY;

		rotation.yaw += xOffset;
		rotation.pitch += yOffset;

		if (rotation.pitch > 89.f)
		{
			rotation.pitch = 89.f;
		}
		else if (rotation.pitch < -89.f)
		{
			rotation.pitch = -89.f;
		}

		glm::vec3 direction;
		direction.x = cos(glm::radians(rotation.yaw)) * cos(glm::radians(rotation.pitch));
		direction.y = sin(glm::radians(rotation.pitch));
		direction.z = sin(glm::radians(rotation.yaw)) * cos(glm::radians(rotation.pitch));
		front = glm::normalize(direction);
		right = glm::normalize(glm::cross(front, up));
	}
}

void Camera::UpdateTranslation(float deltaTime)
{
	if (!isLocked)
	{
		movement = glm::vec3(0.0f);

		if (inputManager->IsKeyDown(GLFW_KEY_W))
		{
			movement += glm::normalize(front) * CAMERA_SPEED * deltaTime;
			movement.y = 0;
			oldPos = pos;
		}

		if (inputManager->IsKeyDown(GLFW_KEY_S))
		{
			movement -= glm::normalize(front) * CAMERA_SPEED * deltaTime;
			movement.y = 0;
			oldPos = pos;
		}

		if (inputManager->IsKeyDown(GLFW_KEY_A))
		{
			movement += glm::normalize(glm::cross(up, front)) * CAMERA_SPEED * deltaTime;
			movement.y = 0;
			oldPos = pos;
		}

		if (inputManager->IsKeyDown(GLFW_KEY_D))
		{
			movement -= glm::normalize(glm::cross(up, front)) * CAMERA_SPEED * deltaTime;
			movement.y = 0;
			oldPos = pos;
		}
	}
}

void Camera::SetAxisValue(char axis, float value) noexcept
{
	if (axis == 'X' || axis == 'x')
	{
		pos.x = value;
	}
	else if (axis == 'Y' || axis == 'y')
	{
		pos.y = value;
	}
	else if (axis == 'Z' || axis == 'z')
	{
		pos.z = value;
	}
}