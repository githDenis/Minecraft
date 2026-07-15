#include "Application.h"
#include "Actor.h"
#include "Texture.h"
#include "World.h"

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

void Application::SetWindow(Window* window) noexcept
{
	this->window = window;
}

void Application::SetInputManager(InputManager* inputManager) noexcept
{
	this->inputManager = inputManager;
}

void Application::SetRender(Render* render) noexcept
{
	this->render = render;
}

void Application::SetShaderProgram(ShaderProgram* shaderProgram) noexcept
{
	this->shaderProgram = shaderProgram;
}

void Application::SetUIShaderProgram(ShaderProgram* shaderProgram) noexcept
{
	UIShaderProgram = shaderProgram;
}

void Application::SetPlayer(Player* player) noexcept
{
	this->player = player;
}

void Application::Run()
{
	//16x16 texture
	Texture texture;
	texture.Create();
	texture.SetImage("E:\\C++\\Minecraft\\Textures\\Textures.jpg");

	UV uvs[Chunck::BLOCKS_TYPES_COUNT][Chunck::UVS_COUNT] = {
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

	Texture textTexture;
	textTexture.Create();
	textTexture.SetImage("E:\\C++\\Minecraft\\Textures\\Font.jpg");

	UIMesh targetMesh;
	targetMesh.GenerateCrossTarget(window->GetWidth(), window->GetHeight());
	targetMesh.SetColor(Color(1.f, 1.f, 1.f));
	targetMesh.Init();

	UIActor targetActor;
	targetActor.SetMesh(&targetMesh);

	world.GenerateChuncksPositions(player->GetPosition());
	world.GenerateChuncks(&texture);
	world.GenerateFolliage();
	world.GenerateChuncksMeshes(uvs);

	player->InitInventory(&textTexture);

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

		int chunkX = static_cast<int>(std::floor(pos.x / (float)Chunck::CHUNK_WIDTH));
		int chunkY = static_cast<int>(std::floor(pos.z / (float)Chunck::CHUNK_LENGTH));

		static int oldChunckX = chunkX;
		static int oldChunckY = chunkY;

		int chunckDx = chunkX - oldChunckX;
		int chunckDy = chunkY - oldChunckY;

		if (chunckDx != 0 || chunckDy != 0)
		{
			glm::vec2 newPos{ chunkX - World::DRAW_CHUNK_RADIUS, chunkY - World::DRAW_CHUNK_RADIUS };

			world.RegenerateWorld(newPos, pos, chunckDx, chunckDy, uvs);

			oldChunckX = chunkX;
			oldChunckY = chunkY;
		}

		world.DrawChuncks(render);

		world.SimulatePhysicsForDroppedBlocks(deltaTime);
		world.ProcessCollisionForDroppedBlocks();
		world.ProcessRotationForDroppedBlocks(deltaTime);
		world.ProcessCollisionWithPlayerForDroppedBlocks(player, &texture, uvs);
		world.DrawDroppedBlocks(render);

		player->UpdatePhysics(deltaTime);
		player->ProcessCollision(&world);

		UIShaderProgram->Use();

		if (!player->IsInventoryUsing())
		{
			inputManager->EnableGamemode();
			targetActor.SetPenSize(3.f);
			render->DrawUIActor(targetActor, GL_LINES);
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