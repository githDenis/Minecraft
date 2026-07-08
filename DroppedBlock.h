#pragma once
#include "Framework.h"
#include "Mesh.h"
#include "Actor.h"
#include "Render.h"
#include"Chunck.h"

class DroppedBlock
{
private:
	Mesh mesh;
	Actor actor;

public:
	void Init(UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT], const Texture* texture, const BlockType& blockType,
		const glm::vec3& blockPos);
	void Draw(Render* render);

    DroppedBlock& operator=(DroppedBlock&& another) noexcept
    {
        mesh = std::move(another.mesh);
        actor = std::move(another.actor);
        actor.SetMesh(&mesh);
        return *this;
    }
};