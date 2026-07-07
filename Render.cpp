#include "Render.h"

Render::Render(Window* window, ShaderProgram* shaderProgram, ShaderProgram* UIshaderProgram) noexcept
{
	this->window = window;
	this->shaderProgram = shaderProgram;
	this->UIShaderProgram = UIshaderProgram;
}

void Render::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}

void Render::Clear(const Color& color)
{
	glClearColor(color.red, color.green, color.blue, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::SetViewport()
{
	glViewport(0, 0, window->GetWidth(), window->GetHeight());
}

void Render::DrawActor(Actor& actor, bool isTransparent)
{
	shaderProgram->SetMatrix4VariableValue("model", actor.GetModelMattrix());

	if (isTransparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	glBindTexture(GL_TEXTURE_2D, actor.GetTexture());
	glBindVertexArray(actor.GetMesh()->GetVAO());
	glDrawElements(GL_TRIANGLES, actor.GetMesh()->GetIndeciesArraySize(), GL_UNSIGNED_INT, NULL);

	if (isTransparent)
	{
		glDisable(GL_BLEND);
	}
}

void Render::DrawUIActor(UIActor& actor, int mode)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	UIShaderProgram->SetMatrix4VariableValue("model", actor.GetModelMatrix());

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(actor.GetMesh()->GetVAO());

	int vertexCount = mode == GL_LINES ? actor.GetMesh()->GetVertexArraySize() / 2 : actor.GetMesh()->GetVertexArraySize() - 2;
	glDrawArrays(mode, 0, vertexCount);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void Render::ApplyCameraData(const Camera& camera)
{
	shaderProgram->SetMatrix4VariableValue("view", camera.GetViewMatrix());
	shaderProgram->SetMatrix4VariableValue("projection", camera.GetProjectionMatrix(window->GetWidth(), window->GetHeight()));
}