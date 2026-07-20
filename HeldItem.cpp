#include "HeldItem.h"

void HeldItem::UpdatePosition(Camera* camera)
{
	glm::vec3 pos = camera->GetPosition()
		+ camera->GetRightMovementVector()
		+ camera->GetUpMovementVector() * 0.1f
		+ camera->GetFrontMovementVector();

	actor.SetPosition(pos);
}

void HeldItem::UpdateRotation(Camera* camera)
{
	Rotator rot = camera->GetRotation();
	rot.yaw = -rot.yaw;
	rot.pitch = -rot.pitch;

	actor.SetRotation(rot + rotation);
}

void HeldItem::Draw(Render* render)
{
	render->DrawActor(actor, false);
}