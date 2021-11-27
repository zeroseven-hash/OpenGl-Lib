#include "pch.h"
#include "Renderer.h"

#include<glad/glad.h>
namespace IGame
{
	glm::mat4 Renderer::s_ViewProjectMatrix = glm::mat4(1.0f);
	void Renderer::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::Clear()
	{
		//TODO:add parma
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void Renderer::Begin(const Camera& camera)
	{
		s_ViewProjectMatrix = camera.GetViewProjection();
	}


	void Renderer::Draw(Shader& shader, const Ref<VertexArrayBuffer>& vao, const glm::mat4& transform)
	{

		vao->Bind();

		shader.Bind();
		shader.SetMat4("u_ProjectMatrix", s_ViewProjectMatrix);
		shader.SetMat4("u_ModelMatrix", transform);
		glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}


	

	void Renderer::end()
	{

	}
	void Renderer::SetViewProjectMatrix(const glm::mat4& viewProjectMatrix)
	{
		s_ViewProjectMatrix = viewProjectMatrix;
	}
	void Renderer::SetViewProjectMatrix(glm::mat4&& viewProjectMatrix)
	{
		s_ViewProjectMatrix = std::move(viewProjectMatrix);
	}
}

