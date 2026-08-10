#pragma once

#include "Structs.h"

enum class BlockType : unsigned char {
	BT_GROUND_GRASS,
	BT_GROUND,
	BT_WOOD,
	BT_LEAVES,
	BT_STONE,
	BT_SAND,
	BT_COAL_ORE,
	BT_IRON_ORE,
	BT_PLANKS,
	BT_CRAFTING_TABLE,
	BT_GRASS,
	BT_YELLOW_FLOWER,
	BT_RED_FLOWER,
	BT_STICK,
	BT_WATER,
	BT_AIR,
};

enum class BlockRenderClass : unsigned char {
	BC_OPAQUE,
	BC_FOLLIAGE,
	BC_TRANSPARENT
};

const int BLOCKS_COUNT = static_cast<int>(BlockType::BT_AIR);
const int UVS_COUNT = 3;

using BlockUVs = UV[BLOCKS_COUNT][UVS_COUNT];