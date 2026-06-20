#include "Application.h"
#include "Actor.h"
#include "Texture.h"
#include "World.h"
#include "Chunck.h"

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
	glfwMakeContextCurrent(this->window->GetHandle());
}

void Application::SetInputManager(InputManager* inputManager) noexcept
{
	this->inputManager = inputManager;
}

void Application::SetRender(Render* render) noexcept
{
	this->render = render;
}

void Application::SetCamera(Camera* camera) noexcept
{
	this->camera = camera;
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
		{ texture.GetUV(0, 16), texture.GetUV(3, 16), texture.GetUV(2, 16) },    //Ground with grass
		{ texture.GetUV(2, 16), texture.GetUV(2, 16), texture.GetUV(2, 16) },    //Ground
		{ texture.GetUV(21, 16), texture.GetUV(20, 16), texture.GetUV(21, 16) }, //Tree
		{ texture.GetUV(53, 16), texture.GetUV(53, 16), texture.GetUV(53, 16) }, //Leaves
		{ texture.GetUV(16, 16), texture.GetUV(16, 16), texture.GetUV(16, 16) }, //Stone
		{ texture.GetUV(142, 16), texture.GetUV(142, 16), texture.GetUV(142, 16) }, //Sand
		{ texture.GetUV(39, 16), texture.GetUV(39, 16), texture.GetUV(53, 16) }, //Grass
		{ texture.GetUV(13, 16), texture.GetUV(13, 16), texture.GetUV(13, 16) }, //Yellow flower
		{ texture.GetUV(12, 16), texture.GetUV(12, 16), texture.GetUV(12, 16) }, //Red flower
	};

	UIMesh targetMesh;
	targetMesh.GenerateCrossTarget(window->GetWidth(), window->GetHeight());

	UIActor targetActor;
	targetActor.SetMesh(&targetMesh);

	world.GenerateChuncksPositions();
	world.GenerateChuncks(&texture);
	world.GenerateFolliage();
	world.GenerateChuncksMeshes(uvs);

	float lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window->GetHandle()))
	{	
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		if (inputManager->GetKeyState(GLFW_KEY_ESCAPE))
		{
			break;
		}

		static const Color clearColor = { 0.f, 0.5f, 0.8f };
		render->Clear(clearColor);

		shaderProgram->Use();

		camera->UpdateTranslation(deltaTime);

		render->ApplyCameraData(*camera);

		Vector3 pos{ player->GetPosition() };

		int chunckX = (int)pos.x / Chunck::CHUNK_WIDTH;
		int chunckY = (int)pos.z / Chunck::CHUNK_LENGTH;

		static int oldChunckX = chunckX;
		static int oldChunckY = chunckY;

		int dx = chunckX - oldChunckX;
		int dy = chunckY - oldChunckY;

		if (dx != 0 || dy != 0)
		{
			Vector2 newPos{ chunckX - World::DRAW_CHUNK_RADIUS, chunckY - World::DRAW_CHUNK_RADIUS };

			world.RegenerateWorld(newPos, dx, dy, uvs);

			oldChunckX = chunckX;
			oldChunckY = chunckY;
		}

		for (int i = 0; i < World::CHUNCKS_COUNT; i++)
		{
			world.DrawChunck(render, i);
		}

		if (inputManager->GetKeyState(GLFW_KEY_SPACE) && player->IsOnGroundState())
		{
			player->Jump();
		}

		if (inputManager->IsKeyHoldForTime(GLFW_KEY_B, 2))
		{
		}
		
		//player->UpdatePhysics(deltaTime);
		player->ProcessCollision(&world);

		std::cout << world.GetBlockType(pos) << '\n';

		UIShaderProgram->Use();
		targetActor.SetPenSize(3.f);
		render->DrawUIActor(targetActor);
		
		glfwSwapBuffers(window->GetHandle());
		glfwPollEvents();
	}
}