#pragma once

#include "Framework.h"
#include "BaseInventory.h"
#include <array>

// |0||1||2|
// |3||4||5|
// |6||7||8|
struct Recipe
{
	std::array<BlockType, 9> input;
	BlockType outputBlock;
	int countCoefficient;
};

class CraftingSystem
{
private:
	static const int RECIPES_COUNT = 3;

	std::array<Recipe, RECIPES_COUNT> recipes
	{
		Recipe {
			{ 
				BlockType::BT_WOOD, BlockType::BT_AIR, BlockType::BT_AIR, 
				BlockType::BT_AIR, BlockType::BT_AIR, BlockType::BT_AIR, 
				BlockType::BT_AIR, BlockType::BT_AIR, BlockType::BT_AIR
			},
			BlockType::BT_PLANKS,
			4,
		},

		Recipe {
			{ 
				BlockType::BT_PLANKS, BlockType::BT_AIR, BlockType::BT_AIR,
				BlockType::BT_PLANKS, BlockType::BT_AIR, BlockType::BT_AIR,
				BlockType::BT_AIR, BlockType::BT_AIR, BlockType::BT_AIR
			},
			BlockType::BT_STICK,
			4,
		},

		Recipe {
			{ 
				BlockType::BT_PLANKS, BlockType::BT_PLANKS, BlockType::BT_AIR,
				BlockType::BT_PLANKS, BlockType::BT_PLANKS, BlockType::BT_AIR,
				BlockType::BT_AIR, BlockType::BT_AIR, BlockType::BT_AIR
			},
			BlockType::BT_CRAFTING_TABLE,
			1,
		},
	};

	int countCoeff = 0;
	BlockType outputBlockType;

public:
	int GetCurrentCountCoeff() const noexcept
	{
		return countCoeff;
	}

	BlockType GetCurrentOutputBlockType() const noexcept
	{
		return outputBlockType;
	}

	bool FindRecipe(std::array<BlockType, 9> currentRecipe) noexcept;
};