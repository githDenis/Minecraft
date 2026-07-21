#pragma once
#include "Framework.h"
#include "Mesh.h"
#include "Actor.h"
#include "Render.h"
#include "Chunk.h"

class HeldItem
{
protected:
	static constexpr Rotator rotation{ 90.f, 180.f, 190.f };
	static constexpr float pitchShakeAngle = 8.f;
	float pitchShakeK = -1.5f;
	float yawShakeK = -2.f;

	Mesh mesh;
	Actor actor;
	bool isShaking = false;

public:
	void StartShaking() noexcept
	{
		isShaking = true;
	}

	void StopShaking() noexcept
	{
		isShaking = false;
	}

	void UpdatePosition(Camera* camera);
	void UpdateRotation(Camera* camera);
	void Draw(Render* render);
};