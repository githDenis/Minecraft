#include "Camera.h"

void Camera::Update()
{
	glm::vec2 mousePos = inputManager->GetMousePosition();
	double x = mousePos.x;
	double y = mousePos.y;

	if (!isLocked)
	{
		static float xLast = 0;
		static float yLast = 0;
		float xOffset = static_cast<float>(x) - xLast;
		float yOffset = yLast - static_cast<float>(y);
		xLast = static_cast<float>(x);
		yLast = static_cast<float>(y);

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

glm::vec3 Camera::GetSignMovementVector() const noexcept
{
	glm::vec3 newVec{ movement.x, movement.y, movement.z };
	if (newVec.x >= 0.08) newVec.x = 1.f;
	else if (newVec.x <= -0.08) newVec.x = -1.f;
	else newVec.x = 0;

	if (newVec.y >= 0.08) newVec.y = 1.f;
	else if (newVec.y <= -0.08) newVec.y = -1.f;
	else newVec.y = 0.f;

	if (newVec.z >= 0.08) newVec.z = 1.f;
	else if (newVec.z <= -0.08) newVec.z = -1.f;
	else newVec.z = 0.f;

	return newVec;
}