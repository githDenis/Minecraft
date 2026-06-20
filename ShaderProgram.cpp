#include "ShaderProgram.h"

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(shaderProgram);
}

void ShaderProgram::Create()
{
	shaderProgram = glCreateProgram();
}

void ShaderProgram::AddShader(const Shader& shader)
{
	glAttachShader(shaderProgram, shader.GetID());
}

void ShaderProgram::Link()
{
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << infoLog << '\n';
	}
	else
	{
		std::cout << "Shader program linking completed successfully\n";
	}
}

void ShaderProgram::Use()
{
	glUseProgram(shaderProgram);
}

void ShaderProgram::SetMatrix4VariableValue(const char* name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, glm::value_ptr(matrix));
}