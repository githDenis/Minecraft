#include "Application.h"
#include "Actor.h"
#include "Texture.h"
#include "World.h"
#include "PlayerHand.h"
#include "HeldBlock.h"
#include "PlayerInventory.h"
#include "CraftingTableInventory.h"

Application::~Application()
{
	glfwTerminate();
}

void Application::Init()
{
	glfwInit();
}

void Application::InitOpenGLContext()
{
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void Application::Run()
{
	//16x16 texture
	Texture texture;
	texture.Create();
	texture.SetImage("D:\\C++\\OpenGL Projects\\Minecraft\\Textures\\Texture.png");

	Texture textTexture;
	textTexture.Create();
	textTexture.SetImage("D:\\C++\\OpenGL Projects\\Minecraft\\Textures\\Font.jpg");

	Texture playerHandTexture;
	playerHandTexture.Create();
	playerHandTexture.SetImage("D:\\C++\\OpenGL Projects\\Minecraft\\Textures\\PlayerHandTexture.jpg");

	BlockUVs uvs = {
		{ texture.GetUV(0, 32, 16), texture.GetUV(3, 32, 16), texture.GetUV(2, 32, 16) },      //Ground with grass
		{ texture.GetUV(2, 32, 16), texture.GetUV(2, 32, 16), texture.GetUV(2, 32, 16) },      //Ground
		{ texture.GetUV(37, 32, 16), texture.GetUV(36, 32, 16), texture.GetUV(37, 32, 16) },   //Tree
		{ texture.GetUV(117, 32, 16), texture.GetUV(117, 32, 16), texture.GetUV(117, 32, 16) },//Leaves
		{ texture.GetUV(1, 32, 16), texture.GetUV(1, 32, 16), texture.GetUV(1, 32, 16) },      //Stone
		{ texture.GetUV(34, 32, 16), texture.GetUV(34, 32, 16), texture.GetUV(34, 32, 16) },   //Sand
		{ texture.GetUV(66, 32, 16), texture.GetUV(66, 32, 16), texture.GetUV(66, 32, 16) },   //Coal ore
		{ texture.GetUV(65, 32, 16), texture.GetUV(65, 32, 16), texture.GetUV(65, 32, 16) },   //Iron ore
		{ texture.GetUV(4, 32, 16), texture.GetUV(4, 32, 16), texture.GetUV(4, 32, 16) },      //Planks
		{ texture.GetUV(75, 32, 16), texture.GetUV(107, 32, 16), texture.GetUV(75, 32, 16) },  //Crafting table
		{ texture.GetUV(87, 32, 16), texture.GetUV(87, 32, 16), texture.GetUV(87, 32, 16) },   //Grass
		{ texture.GetUV(29, 32, 16), texture.GetUV(29, 32, 16), texture.GetUV(29, 32, 16) },   //Yellow flower
		{ texture.GetUV(28, 32, 16), texture.GetUV(28, 32, 16), texture.GetUV(28, 32, 16) },   //Red flower
		{ texture.GetUV(308, 32, 16), texture.GetUV(308, 32, 16), texture.GetUV(308, 32, 16) },//Stick
		{ texture.GetUV(369, 32, 16), texture.GetUV(369, 32, 16), texture.GetUV(369, 32, 16) },//Water
	};

	player->SetHandTexture(&playerHandTexture);

	UIActor targetActor;
	targetActor.GetMesh().GenerateCrossTarget(window->GetWidth(), window->GetHeight());
	targetActor.GetMesh().SetColor(Color(1.f, 1.f, 1.f));
	targetActor.GetMesh().Init();

	world.GenerateChunksPositions(player->GetPosition());
	world.GenerateChunks(&texture);
	world.GenerateFolliage();
	world.GenerateChunksMeshes(uvs);

	player->SetPlayerInventory(std::make_unique<PlayerInventory>(player, craftingSystem, window, &textTexture, 
		PlayerInventory::INVENTORY_WIDTH, PlayerInventory::INVENTORY_HEIGHT));
	
	float lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window->GetHandle()))
	{	
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		shaderProgram->Use();

		///////////////////////////////////// SPACE ///////////////////////////////////
		if (inputManager->IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			break;
		}

		if (inputManager->IsKeyDown(GLFW_KEY_SPACE) && player->IsOnGroundState())
		{
			player->Jump();
		}
		///////////////////////////////////// SPACE ///////////////////////////////////


		///////////////////////////////////// I ///////////////////////////////////
		if (inputManager->IsKeyPressed(GLFW_KEY_I))
		{
			player->UseInventory();

			if (player->IsInventoryUsing())
			{
				inputManager->SetCursorPosition(window->GetWidth() / 2, window->GetHeight() / 2);
			}
			else
			{
				inputManager->SetCursorPosition(window->GetWidth() / 2, window->GetHeight() / 2);

				if (player->IsInteracting())
				{
					player->CopyItemsFromItemToPlayerInvntory();
				}
			}

			if (player->IsInteracting())
			{
				player->SetInteractingState(false);
			}
		}
		///////////////////////////////////// I ///////////////////////////////////


		///////////////////////////////////// MLB ///////////////////////////////////
		if (inputManager->IsMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (player->IsInventoryUsing())
			{
				if (!player->IsInteracting())
				{
					player->GetPlayerInventory()->ProcessMouseRelease(&world, &texture, &textTexture, uvs);
					player->GetPlayerInventory()->CheckCrafting(&world, &texture, uvs);
				}
				else
				{
					player->GetItemInventory()->ProcessMouseRelease(&world, &texture, &textTexture, uvs);
					
					if (auto inventory = dynamic_cast<CraftingTableInventory*>(player->GetItemInventory().get()))
					{
						inventory->CheckCrafting(&world, &texture, uvs);
					}
				}
			}
			else
			{
				player->StopShakingHeldItem();
			}
		}

		if (inputManager->IsMouseButtonHoldForTime(GLFW_MOUSE_BUTTON_LEFT, 800))
		{
			if (!player->IsInventoryUsing())
			{
				player->StartShakingHeldItem();
				player->DestroyBlock(&world, uvs, &texture, render);
			}

		}

		if (inputManager->IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (!player->IsInteracting())
			{
				player->GetPlayerInventory()->ProcessMouseCkick(inputManager, &texture, &textTexture, uvs);
			}
			else
			{
				player->GetItemInventory()->ProcessMouseCkick(inputManager, &texture, &textTexture, uvs);
			}
		}
		///////////////////////////////////// MLB ///////////////////////////////////


		///////////////////////////////////// MRB ///////////////////////////////////
		if (inputManager->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			if (!player->IsInventoryUsing())
			{
				BlockType blockType = player->StartLineTracing(&world);

				if (!world.IsBlockInteractable(blockType))
				{
					player->PlaceBlock(&world, render, uvs);
				}
				else
				{
					player->UseInventory();
					player->SetInteractingState(true);

					if (blockType == BlockType::BT_CRAFTING_TABLE)
					{
						static int count = 0;
						count++;

						if (count >= 2)
						{
							count = 0;
						}

						player->SetItemInventory(std::make_unique<CraftingTableInventory>(
							craftingSystem, window,  &textTexture, CraftingTableInventory::INVENTORY_WIDTH,
							CraftingTableInventory::INVENTORY_HEIGHT));
						player->CopyItemsFromPlayerToItemInvntory();
					}
				}
			}
		}
		///////////////////////////////////// MRB ///////////////////////////////////


		///////////////////////////////////// Q ///////////////////////////////////
		if (inputManager->IsKeyPressed(GLFW_KEY_Q))
		{
			if (!player->IsInventoryUsing())
			{
				player->GetPlayerInventory()->ThrowOutItemFromHotbar(&world, &texture, uvs);
			}
			else
			{
				player->GetPlayerInventory()->ThrowOutItemFromInventory(inputManager, &world, &texture, uvs);
			}
		}
		///////////////////////////////////// Q ///////////////////////////////////


		///////////////////////////////////// LEFT SHIFT ///////////////////////////////////
		if (inputManager->IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			if (!player->IsInteracting())
			{
				if (player->GetPlayerInventory()->IsItemDragging())
				{
					player->GetPlayerInventory()->SplitItems(&texture, uvs);
				}
			}
			else
			{
				if (player->GetItemInventory()->IsItemDragging())
				{
					player->GetItemInventory()->SplitItems(&texture, uvs);
				}
			}
		}
		///////////////////////////////////// LEFT SHIFT ///////////////////////////////////

		int scrollDelta = inputManager->GetMouseScrollDelta();

		if (scrollDelta > 0)
		{
			player->GetPlayerInventory()->SelectLeftItem();
		}

		if (scrollDelta < 0)
		{
			player->GetPlayerInventory()->SelectRightItem();
		}

		static const Color clearColor = { 0.f, 0.5f, 0.8f };
		render->Clear(clearColor);

		player->UpdateCamera(deltaTime);
		render->ApplyCameraData(player->GetCamera());

		glm::vec3 pos{ player->GetPosition() };

		int chunkX = static_cast<int>(std::floor(pos.x / (float)Chunk::CHUNK_WIDTH));
		int chunkY = static_cast<int>(std::floor(pos.z / (float)Chunk::CHUNK_LENGTH));

		static int oldChunkX = chunkX;
		static int oldChunkY = chunkY;

		int chunkDx = chunkX - oldChunkX;
		int chunkDy = chunkY - oldChunkY;

		if (chunkDx != 0 || chunkDy != 0)
		{
			glm::vec2 newPos{ chunkX - World::DRAW_CHUNK_RADIUS, chunkY - World::DRAW_CHUNK_RADIUS };

			world.RegenerateWorld(newPos, pos, chunkDx, chunkDy, uvs);

			oldChunkX = chunkX;
			oldChunkY = chunkY;
		}

		world.DrawChunks(render);

		world.SimulatePhysicsForDroppedBlocks(deltaTime);
		world.ProcessCollisionForDroppedBlocks();
		world.ProcessRotationForDroppedBlocks(deltaTime);
		world.ProcessCollisionWithPlayerForDroppedBlocks(player, &texture, uvs);
		world.DrawDroppedBlocks(render);

		player->UpdatePhysics(deltaTime);
		player->ProcessCollision(&world);

		player->UpdateHeldItem(uvs);
		player->DrawHeldItem(render);

		UIShaderProgram->Use();

		if (!player->IsInventoryUsing())
		{
			inputManager->EnableGamemode();
			targetActor.SetPenSize(3.f);
			render->DrawUIActor(targetActor, GL_LINES);
			player->GetPlayerInventory()->ShowHotBar(render);
			player->GetPlayerInventory()->ShowCurrentItemFrame(render);
		}
		else
		{
			inputManager->EnableUIMode();

			if (!player->IsInteracting())
			{
				player->GetPlayerInventory()->Draw(render);
				player->GetPlayerInventory()->ProcessMouseHovering(inputManager, render);
				player->GetPlayerInventory()->UpdateDraggingItemPosition(inputManager);
				player->GetPlayerInventory()->ShowDraggingItem(render);
			}
			else
			{
				player->GetItemInventory()->Draw(render);
				player->GetItemInventory()->ProcessMouseHovering(inputManager, render);
				player->GetItemInventory()->UpdateDraggingItemPosition(inputManager);
				player->GetItemInventory()->ShowDraggingItem(render);
			}
		}
		glfwSwapBuffers(window->GetHandle());
		glfwPollEvents();
	}
}