#include "CraftingSystem.h"

bool CraftingSystem::FindRecipe(std::array<BlockType, 9> currentRecipe) noexcept
{
	bool isRecipeFound = false;

	for (int i = 0; i < recipes.size(); i++)
	{
		for (int j = 0; j < recipes[0].input.size(); j++)
		{
			if (recipes[i].input[j] == currentRecipe[j])
			{
				if (j == recipes[0].input.size() - 1)
				{
					isRecipeFound = true;
					countCoeff = recipes[i].countCoefficient;
					outputBlockType = recipes[i].outputBlock;
					break;
				}
				continue;
			}
			else
			{
				break;
			}
		}
	}
	return isRecipeFound;
}