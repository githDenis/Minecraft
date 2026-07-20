#pragma once

#include "Framework.h"
#include "InputManager.h"
#include "Structs.h"

enum class Direction : unsigned char
{
	RIGHT, LEFT,
	UP, DOWN,
	FORWARD, BACKWARD,
};

class Camera
{
public:
	static constexpr float CAMERA_SPEED = 10.f;
	static constexpr float SENSITIVITY = 0.05f;
	static constexpr float DRAWING_DISTANCE = 200.f;

private:
	InputManager* inputManager;

	glm::vec3 movement;
	glm::vec3 pos = glm::vec3(50, 50, 50);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 front = glm::vec3(0, 0, -1);
	glm::vec3 right;
	glm::vec3 oldPos = glm::vec3(50, 50, 50);
	Rotator rotation{ 0, 0, 0 };
	float FOV;
	bool isLocked = false;

public:
	void SetInputManager(InputManager* inputManger) noexcept
	{
		this->inputManager = inputManger;
	}

	void SetFOV(float angle) noexcept
	{
		FOV = angle;
	}

	void SetLockedState(bool state) noexcept
	{
		isLocked = state;
	}

	void SetPosition(const glm::vec3& vector) noexcept
	{
		oldPos = pos;
		pos = vector;
	}

	glm::mat4 GetViewMatrix() const noexcept
	{
		return glm::lookAt(pos, pos + front, up);
	}

	glm::mat4 GetProjectionMatrix(float windowWidth, float windowHeight) const noexcept
	{
		return glm::perspective(glm::radians(FOV), windowWidth / windowHeight, 0.1f, DRAWING_DISTANCE);
	}

	const glm::vec3& GetPosition() const noexcept
	{
		return pos;
	}

	const glm::vec3& GetMovementVector() const noexcept
	{
		return movement;
	}

	const glm::vec3& GetFrontMovementVector() const noexcept
	{
		return front;
	}

	const glm::vec3& GetUpMovementVector() const noexcept
	{
		return up;
	}

	const glm::vec3& GetRightMovementVector() const noexcept
	{
		return right;
	}

	const glm::vec3& GetOldPosition() const noexcept
	{
		return oldPos;
	}

	Rotator& GetRotation() noexcept
	{
		return rotation;
	}

	void Update();
	void UpdateTranslation(float deltaTime);
	void SetAxisValue(char axis, float value) noexcept;
	
	glm::vec3 GetSignMovementVector() const noexcept;
};