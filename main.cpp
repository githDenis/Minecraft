#include "Framework.h"
#include "Application.h"
#include "Window.h"
#include "InputManager.h"
#include "Render.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Camera.h"
#include "Player.h"

const int WINDOW_WIDTH = 1800;
const int WINDOW_HEIGHT = 900;

int main()
{
	Application app;
	app.Init();

	ShaderProgram shaderProgram;
	ShaderProgram UIShaderProgram;

	Window window("Game window", WINDOW_WIDTH, WINDOW_HEIGHT);
	InputManager inputManager(&window);
	inputManager.EnableGamemode();

 	Render render(&window, &shaderProgram, &UIShaderProgram);
	Camera camera(&inputManager);
	camera.SetFOV(45.f);
	camera.InitMouseMoveCallback();

	Player player(&camera);
	
	app.SetWindow(&window);
	app.SetInputManager(&inputManager);
	app.SetRender(&render);
	app.SetCamera(&camera);
	app.SetPlayer(&player);
	app.InitOpenGLContext();

	// World Shaders
	Shader vertexShader;
	vertexShader.Create(ShaderType::ST_VERTEX);
	vertexShader.ReadShaderFile("E:\\C++\\Minecraft\\VertexShader.glsl");
	vertexShader.Compile();

	Shader fragmentShader;
	fragmentShader.Create(ShaderType::ST_FRAGMENT);
	fragmentShader.ReadShaderFile("E:\\C++\\Minecraft\\FragmentShader.glsl");
	fragmentShader.Compile();

	shaderProgram.Create();
	shaderProgram.AddShader(vertexShader);
	shaderProgram.AddShader(fragmentShader);
	shaderProgram.Link();
	// World Shaders

	// UI Shaders
	Shader UIVertexShader;
	UIVertexShader.Create(ShaderType::ST_VERTEX);
	UIVertexShader.ReadShaderFile("E:\\C++\\Minecraft\\UIVertexShader.glsl");
	UIVertexShader.Compile();

	Shader UIFragmentShader;
	UIFragmentShader.Create(ShaderType::ST_FRAGMENT);
	UIFragmentShader.ReadShaderFile("E:\\C++\\Minecraft\\UIFragmentShader.glsl");
	UIFragmentShader.Compile();

	UIShaderProgram.Create();
	UIShaderProgram.AddShader(UIVertexShader);
	UIShaderProgram.AddShader(UIFragmentShader);
	UIShaderProgram.Link();
	// UI Shaders
	
	app.SetShaderProgram(&shaderProgram);
	app.SetUIShaderProgram(&UIShaderProgram);

	render.Init();
	render.SetViewport();

	app.Run();
	return 0;
}