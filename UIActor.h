#pragma once

#include "Framework.h"
#include "UIMesh.h"

class UIActor
{
private:
	UIMesh* mesh;

public:
	UIActor() {};

	void SetMesh(UIMesh* mesh) noexcept;
	void SetPenSize(float size) const;
	UIMesh* GetMesh() noexcept;
};