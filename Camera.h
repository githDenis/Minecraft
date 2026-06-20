#pragma once

#include "Framework.h"
#include "InputManager.h"
#include "Structs.h"

enum class AxisDirection : unsigned char
{
	RIGHT, LEFT,
	UP, DOWN,
	FORWARD, BACKWARD,
};

class Camera
{
public:
	static constexpr float CAMERA_SPEED = 12.f;
	static constexpr float SENSITIVITY = 0.05f;
	static constexpr float DRAWING_DISTANCE = 160.f;

private:
	InputManager* inputManager;

	glm::vec3 movement;
	glm::vec3 pos = glm::vec3(50, 50, 50);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 front = glm::vec3(0, 0, -1);
	glm::vec3 oldPos = glm::vec3(50, 50, 50);

	Rotator rotation{ 0, 0, 0 };

	float FOV;

	static void MouseMoveCallback(GLFWwindow* window, double x, double y);
	void OnMouseMove(double x, double y);

public:
	Camera(InputManager* inputManager) noexcept;

	void InitMouseMoveCallback();
	void UpdateTranslation(float deltaTime);
	void SetFOV(float angle) noexcept;
	void SetPosition(const Vector3& vector) noexcept;
	void SetOldPosition(const Vector3& vector) noexcept;

	glm::mat4 GetViewMatrix() const noexcept;
	glm::mat4 GetProjectionMatrix(float windowWidth, float windowHeight) const noexcept;
	
	Vector3 GetPosition() const noexcept;
	Vector3 GetMovementVector() const noexcept;
	Vector3 GetOldPosition() const noexcept;
};