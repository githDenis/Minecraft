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

	Mesh mesh;
	Actor actor;

public:
	virtual void Init(Texture* texture) = 0;
	void UpdatePosition(Camera* camera);
	void UpdateRotation(Camera* camera);
	void Draw(Render* render);
};