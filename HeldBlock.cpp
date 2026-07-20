#include "HeldBlock.h"

void HeldBlock::Init(Texture* texture)
{
	UV uv{ texture->GetUV(0, 16, 16) };
	mesh.GenerateCubeWithOffset(offset);
	mesh.SetCubeUV(uv, uv, uv);
	mesh.InitMesh();

	actor.SetMesh(&mesh);
	actor.SetTexture(texture);
	actor.SetRotation(rotation);
	actor.SetPosition(offset);
	actor.SetScale(scale);
}