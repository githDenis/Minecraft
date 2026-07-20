#pragma once

#include "Framework.h"

enum ShaderType { ST_VERTEX = GL_VERTEX_SHADER, ST_FRAGMENT = GL_FRAGMENT_SHADER };

class Shader
{
private:
	unsigned int shader;
	char* shaderCode;

public:
	unsigned int GetID() const noexcept
	{
		return shader;
	}

	~Shader();
	void Create(unsigned int type);
	void ReadShaderFile(const char* filePath);
	void Compile() const;
};