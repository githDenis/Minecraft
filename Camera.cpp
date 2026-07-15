#include "Camera.h"

void Camera::SetInputManager(InputManager* inputManger) noexcept
{
	this->inputManager = inputManger;
}

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

void Camera::SetFOV(float angle) noexcept
{
	FOV = angle;
}

void Camera::SetLockedState(bool state) noexcept
{
	isLocked = state;
}

void Camera::SetPosition(const glm::vec3& vector) noexcept
{
	oldPos = pos;
	pos = vector;
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

glm::mat4 Camera::GetViewMatrix() const noexcept
{
	return glm::lookAt(pos, pos + front, up);
}

glm::mat4 Camera::GetProjectionMatrix(float windowWidth, float windowHeight) const noexcept
{
	return glm::perspective(glm::radians(FOV), windowWidth / windowHeight, 0.1f, DRAWING_DISTANCE);
}

const glm::vec3& Camera::GetPosition() const noexcept
{
	return pos;
}

const glm::vec3& Camera::GetMovementVector() const noexcept
{
	return movement;
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

const glm::vec3& Camera::GetFrontMovementVector() const noexcept
{
	return front;
}

const glm::vec3& Camera::GetOldPosition() const noexcept
{
	return oldPos;
}