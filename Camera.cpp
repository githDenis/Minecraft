#include "Camera.h"

Camera::Camera(InputManager* inputManager) noexcept
{
	this->inputManager = inputManager;
}

void Camera::MouseMoveCallback(GLFWwindow* window, double x, double y)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

	if (camera)
	{
		camera->OnMouseMove(x, y);
	}
}

void Camera::OnMouseMove(double x, double y)
{
	static float xLast = 0;
	static float yLast = 0;
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
}

void Camera::InitMouseMoveCallback()
{
	glfwSetWindowUserPointer(inputManager->GetWindow(), this);
	inputManager->BindMouseCallback(&Camera::MouseMoveCallback);
}

void Camera::UpdateTranslation(float deltaTime)
{
	movement = glm::vec3(0.0f);

	if (inputManager->GetKeyState(GLFW_KEY_W))
	{
		movement += glm::normalize(front) * CAMERA_SPEED * deltaTime;
		oldPos = pos;
	}

	if (inputManager->GetKeyState(GLFW_KEY_S))
	{
		movement -= glm::normalize(front) * CAMERA_SPEED * deltaTime;
		oldPos = pos;
	}

	if (inputManager->GetKeyState(GLFW_KEY_A))
	{
		movement += glm::normalize(glm::cross(up, front)) * CAMERA_SPEED * deltaTime;
		oldPos = pos;
	}

	if (inputManager->GetKeyState(GLFW_KEY_D))
	{
		movement -= glm::normalize(glm::cross(up, front)) * CAMERA_SPEED * deltaTime;
		oldPos = pos;
	}
}

void Camera::SetFOV(float angle) noexcept
{
	FOV = angle;
}

void Camera::SetPosition(const Vector3& vector) noexcept
{
	pos.x = vector.x;
	pos.y = vector.y;
	pos.z = vector.z;
}

void Camera::SetOldPosition(const Vector3& vector) noexcept
{
	oldPos.x = vector.x;
	oldPos.y = vector.y;
	oldPos.z = vector.z;
}

glm::mat4 Camera::GetViewMatrix() const noexcept
{
	return glm::lookAt(pos, pos + front, up);
}

glm::mat4 Camera::GetProjectionMatrix(float windowWidth, float windowHeight) const noexcept
{
	return glm::perspective(glm::radians(FOV), windowWidth / windowHeight, 0.1f, DRAWING_DISTANCE);
}

Vector3 Camera::GetPosition() const noexcept
{
	return Vector3{ pos.x, pos.y, pos.z };
}

Vector3 Camera::GetMovementVector() const noexcept
{
	return Vector3{ movement.x, movement.y, movement.z };
}

Vector3 Camera::GetOldPosition() const noexcept
{
	return Vector3{ oldPos.x, oldPos.y, oldPos.z };
}