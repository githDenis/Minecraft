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
#include "CraftingSystem.h"

int main()
{
	Application app;
	app.Init();

	Window window("Game window");
	window.MakeCurrent();

	ShaderProgram shaderProgram;
	ShaderProgram folliageShaderProgram;
	ShaderProgram UIShaderProgram;

	InputManager inputManager(&window);
	inputManager.BindMouseCallback();
	inputManager.BindMouseScrollCallback();
	inputManager.EnableGamemode();

 	Render render(&window, &shaderProgram, &folliageShaderProgram, &UIShaderProgram);

	Player player(&window, &inputManager);
	CraftingSystem craftingSystem;

	app.SetWindow(&window);
	app.SetInputManager(&inputManager);
	app.SetRender(&render);
	app.SetPlayer(&player);
	app.SetCraftingSystem(&craftingSystem);
	app.InitOpenGLContext();

	// World Shaders
	Shader vertexShader;
	vertexShader.Create(ShaderType::ST_VERTEX);
	vertexShader.ReadShaderFile("D:\\C++\\OpenGL Projects\\Minecraft\\Shaders\\VertexShader.glsl");
	vertexShader.Compile();

	Shader fragmentShader;
	fragmentShader.Create(ShaderType::ST_FRAGMENT);
	fragmentShader.ReadShaderFile("D:\\C++\\OpenGL Projects\\Minecraft\\Shaders\\FragmentShader.glsl");
	fragmentShader.Compile();

	shaderProgram.Create();
	shaderProgram.AddShader(vertexShader);
	shaderProgram.AddShader(fragmentShader);
	shaderProgram.Link();
	// World Shaders


	// Folliage Shaders
	Shader folVertexShader;
	folVertexShader.Create(ShaderType::ST_VERTEX);
	folVertexShader.ReadShaderFile("D:\\C++\\OpenGL Projects\\Minecraft\\Shaders\\FolliageVertexShader.glsl");
	folVertexShader.Compile();

	Shader folFragmentShader;
	folFragmentShader.Create(ShaderType::ST_FRAGMENT);
	folFragmentShader.ReadShaderFile("D:\\C++\\OpenGL Projects\\Minecraft\\Shaders\\FolliageFragmentShader.glsl");
	folFragmentShader.Compile();

	folliageShaderProgram.Create();
	folliageShaderProgram.AddShader(folVertexShader);
	folliageShaderProgram.AddShader(folFragmentShader);
	folliageShaderProgram.Link();
	// Folliage Shaders


	// UI Shaders
	Shader UIVertexShader;
	UIVertexShader.Create(ShaderType::ST_VERTEX);
	UIVertexShader.ReadShaderFile("D:\\C++\\OpenGL Projects\\Minecraft\\Shaders\\UIVertexShader.glsl");
	UIVertexShader.Compile();

	Shader UIFragmentShader;
	UIFragmentShader.Create(ShaderType::ST_FRAGMENT);
	UIFragmentShader.ReadShaderFile("D:\\C++\\OpenGL Projects\\Minecraft\\Shaders\\UIFragmentShader.glsl");
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