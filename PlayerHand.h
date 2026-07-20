#pragma once
#include "HeldItem.h"

class PlayerHand : public HeldItem
{
private:
	static constexpr glm::vec3 offset = glm::vec3(-1.2f, -1.f, -0.1f);
	static constexpr glm::vec3 scale = glm::vec3(0.6f, 0.6f, 4.f);

public:
	virtual void Init(Texture* texture) override;
};