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
	Rotator cameraRot = camera->GetRotation();
	cameraRot.yaw = -cameraRot.yaw;
	cameraRot.pitch = -cameraRot.pitch;

	if (isShaking)
	{
		static Rotator rot = rotation;
		rot.pitch += pitchShakeK;
		rot.yaw += yawShakeK;

		if (rot.pitch < rotation.pitch - pitchShakeAngle || rot.pitch > rotation.pitch + pitchShakeAngle)
		{
			pitchShakeK *= -1.f;
			yawShakeK *= -1.f;
		}
		actor.SetRotation(cameraRot + rot);
	}
	else
	{
		actor.SetRotation(cameraRot + rotation);
	}
}

void HeldItem::Draw(Render* render)
{
	render->DrawActor(actor, false);
}