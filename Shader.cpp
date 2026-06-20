#include "Shader.h"
#include <filesystem>
#include <fstream>

Shader::~Shader()
{
	glDeleteShader(shader);
	
	if (shaderCode)
	{
		delete[] shaderCode;
	}
}

void Shader::Create(unsigned int type)
{
	shader = glCreateShader(type);
}

void Shader::ReadShaderFile(const char* filePath)
{
	int fileSize = std::filesystem::file_size(filePath);

	std::ifstream file(filePath, std::ios::binary);

	if (file)
	{
		shaderCode = new char[fileSize + 1];

		file.read(shaderCode, fileSize + 1);
		shaderCode[fileSize] = '\0';

		file.close();
	}
}

void Shader::Compile() const
{
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cout << infoLog << '\n';
	}
	else
	{
		std::cout << "Shader compilation completed successfully\n";
	}
}

unsigned int Shader::GetID() const noexcept
{
	return shader;
}