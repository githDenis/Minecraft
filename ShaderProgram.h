#pragma once

#include "Framework.h"
#include "Shader.h"

class ShaderProgram
{
private:
	unsigned int shaderProgram;

public:
	~ShaderProgram();

	void Create();
	void AddShader(const Shader& shader);
	void Link();
	void Use();

	void SetMatrix4VariableValue(const char* name, const glm::mat4& matrix);
};