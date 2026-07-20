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
	texture.SetImage("E:\\C++\\Minecraft\\Textures\\Textures.jpg");

	Texture textTexture;
	textTexture.Create();
	textTexture.SetImage("E:\\C++\\Minecraft\\Textures\\Font.jpg");

	Texture playerHandTexture;
	playerHandTexture.Create();
	playerHandTexture.SetImage("E:\\C++\\Minecraft\\Textures\\PlayerHandTexture.jpg");

	UV uvs[Chunk::BLOCKS_COUNT][Chunk::UVS_COUNT] = {
		{ texture.GetUV(0, 16, 16), texture.GetUV(3, 16, 16), texture.GetUV(2, 16, 16) },      //Ground with grass
		{ texture.GetUV(2, 16, 16), texture.GetUV(2, 16, 16), texture.GetUV(2, 16, 16) },      //Ground
		{ texture.GetUV(21, 16, 16), texture.GetUV(20, 16, 16), texture.GetUV(21, 16, 16) },   //Tree
		{ texture.GetUV(53, 16, 16), texture.GetUV(53, 16, 16), texture.GetUV(53, 16, 16) },   //Leaves
		{ texture.GetUV(16, 16, 16), texture.GetUV(16, 16, 16), texture.GetUV(16, 16, 16) },   //Stone
		{ texture.GetUV(142, 16, 16), texture.GetUV(142, 16, 16), texture.GetUV(142, 16, 16) },//Sand
		{ texture.GetUV(39, 16, 16), texture.GetUV(39, 16, 16), texture.GetUV(53, 16, 16) },   //Grass
		{ texture.GetUV(13, 16, 16), texture.GetUV(13, 16, 16), texture.GetUV(13, 16, 16) },   //Yellow flower
		{ texture.GetUV(12, 16, 16), texture.GetUV(12, 16, 16), texture.GetUV(12, 16, 16) },   //Red flower
		{ texture.GetUV(177, 16, 16), texture.GetUV(177, 16, 16), texture.GetUV(177, 16, 16) },//Water
	};

	PlayerHand playerHand;
	playerHand.Init(&playerHandTexture);

	HeldBlock heldBlock;
	heldBlock.Init(&texture);

	HeldItem* items[] = { &playerHand, &heldBlock };

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
	player->SetHeldItem(&heldBlock);

	float lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window->GetHandle()))
	{	
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

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
		}

		if (inputManager->IsMouseButtonHoldForTime(GLFW_MOUSE_BUTTON_LEFT, 800))
		{
			player->DestroyBlock(&world, uvs, &texture, render);
		}

		if (inputManager->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			player->PlaceBlock(&world, render, uvs, BlockType::BT_GROUND);
		}

		static const Color clearColor = { 0.f, 0.5f, 0.8f };
		render->Clear(clearColor);

		shaderProgram->Use();

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

		player->DrawHeldItem(render);

		UIShaderProgram->Use();

		if (!player->IsInventoryUsing())
		{
			inputManager->EnableGamemode();
			targetActor.SetPenSize(3.f);
			render->DrawUIActor(targetActor, GL_LINES);
			player->DrawHotBar(render);
		}
		else
		{
			inputManager->EnableUIMode();
			player->DrawInventory(render);
			player->ProcessHoveringForInventory(inputManager, render);
		}
		glfwSwapBuffers(window->GetHandle());
		glfwPollEvents();
	}
}