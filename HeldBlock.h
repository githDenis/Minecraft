#pragma once
#include "Framework.h"
#include "HeldItem.h"

class HeldBlock : public HeldItem
{
private:
	static constexpr glm::vec3 offset = glm::vec3(-0.6f, -0.3f, -0.f);
	static constexpr glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);

public:
	virtual void Init(Texture* texture) override;
};