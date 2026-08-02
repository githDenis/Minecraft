#include "Application.h"
#include "Actor.h"
#include "Texture.h"
#include "World.h"
#include "PlayerHand.h"
#include "HeldBlock.h"

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
	texture.SetImage("E:\\C++\\Minecraft\\Textures\\Texture.png");

	Texture textTexture;
	textTexture.Create();
	textTexture.SetImage("E:\\C++\\Minecraft\\Textures\\Font.jpg");

	Texture playerHandTexture;
	playerHandTexture.Create();
	playerHandTexture.SetImage("E:\\C++\\Minecraft\\Textures\\PlayerHandTexture.jpg");

	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT] = {
		{ texture.GetUV(0, 32, 16), texture.GetUV(3, 32, 16), texture.GetUV(2, 32, 16) },      //Ground with grass
		{ texture.GetUV(2, 32, 16), texture.GetUV(2, 32, 16), texture.GetUV(2, 32, 16) },      //Ground
		{ texture.GetUV(37, 32, 16), texture.GetUV(36, 32, 16), texture.GetUV(37, 32, 16) },   //Tree
		{ texture.GetUV(117, 32, 16), texture.GetUV(117, 32, 16), texture.GetUV(117, 32, 16) },//Leaves
		{ texture.GetUV(1, 32, 16), texture.GetUV(1, 32, 16), texture.GetUV(1, 32, 16) },      //Stone
		{ texture.GetUV(34, 32, 16), texture.GetUV(34, 32, 16), texture.GetUV(34, 32, 16) },   //Sand
		{ texture.GetUV(4, 32, 16), texture.GetUV(4, 32, 16), texture.GetUV(4, 32, 16) },      //Planks
		{ texture.GetUV(75, 32, 16), texture.GetUV(107, 32, 16), texture.GetUV(75, 32, 16) },  //Crafting table
		{ texture.GetUV(87, 32, 16), texture.GetUV(87, 32, 16), texture.GetUV(87, 32, 16) },   //Grass
		{ texture.GetUV(29, 32, 16), texture.GetUV(29, 32, 16), texture.GetUV(29, 32, 16) },   //Yellow flower
		{ texture.GetUV(28, 32, 16), texture.GetUV(28, 32, 16), texture.GetUV(28, 32, 16) },   //Red flower
		{ texture.GetUV(208, 32, 16), texture.GetUV(208, 32, 16), texture.GetUV(208, 32, 16) },//Stick
		{ texture.GetUV(369, 32, 16), texture.GetUV(369, 32, 16), texture.GetUV(369, 32, 16) },//Water
	};

	player->SetHandTexture(&playerHandTexture);

	UIMesh targetMesh;
	targetMesh.GenerateCrossTarget(window->GetWidth(), window->GetHeight());
	targetMesh.SetColor(Color(1.f, 1.f, 1.f));
	targetMesh.Init();

	UIActor targetActor;
	targetActor.SetMesh(&targetMesh);

	world.GenerateChunksPositions(player->GetPosition());
	world.GenerateChunks(&texture);
	world.GenerateFolliage();
	world.GenerateChunksMeshes(uvs);

	player->InitInventory(&textTexture);
	
	float lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window->GetHandle()))
	{	
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		shaderProgram->Use();

		if (inputManager->IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			break;
		}

		if (inputManager->IsKeyDown(GLFW_KEY_SPACE) && player->IsOnGroundState())
		{
			player->Jump();
		}

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
			}
		}

		if (inputManager->IsMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT))
		{			
			if (player->IsInventoryUsing())
			{
				player->ProcessingMouseRelease(&world, &texture, &textTexture, uvs);
				player->CheckCrafting(&world, &texture, uvs);
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
			else
			{
				player->ProcessingMouseCkick(inputManager, &texture, &textTexture, uvs);
			}
		}

		if (inputManager->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			if (!player->IsInventoryUsing())
			{
				player->PlaceBlock(&world, render, uvs);
			}
		}

		if (inputManager->IsKeyPressed(GLFW_KEY_Q))
		{
			if (!player->IsInventoryUsing())
			{
				player->ThrowOutItemFromHotbar(&world, &texture, uvs);
			}
			else
			{
				player->ThrowOutItemFromInventory(inputManager, &world, &texture, uvs);
			}
		}

		if (inputManager->IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			if (player->IsItemDragging())
			{
				player->SplitItems(&texture, uvs);
			}
		}

		int scrollDelta = inputManager->GetMouseScrollDelta();

		if (scrollDelta > 0)
		{
			player->SelectLeftItem();
		}

		if (scrollDelta < 0)
		{
			player->SelectRightItem();
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
			player->DrawHotBar(render);
			player->DrawCurrentItemFrame(render);
		}
		else
		{
			inputManager->EnableUIMode();
			player->DrawInventory(render);
			player->ProcessHoveringForInventory(inputManager, render);
			player->UpdateDraggingItemPosition(inputManager);
			player->DrawDraggingItem(render);
		}
		glfwSwapBuffers(window->GetHandle());
		glfwPollEvents();
	}
}