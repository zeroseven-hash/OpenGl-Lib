#pragma once

#include"Camera.h"
#include"Shader.h"
#include"VertexArrayBuffer.h"
#include"../DataStructure/DrawableMesh.h"
#include"../DataStructure/DrawableLine.h"
#include"../DataStructure/DrawablePoint.h"

namespace IGame
{
	class Renderer
	{
	public:
		static void Init();
		static void Clear();
		static void SetClearColor(float r, float g, float b, float a);
		static void Begin(const Camera& camera);
		static void end();
		
	
		static void Draw(Shader& shader, const Ref<VertexArrayBuffer>& vao,const glm::mat4& transform);

		template<typename T,typename GLFunc>
		static void Draw(Shader& shader,const Ref<T>& obj,const glm::mat4& transform,GLFunc func);

		static void SetViewProjectMatrix(const glm::mat4& transform);
		static void SetViewProjectMatrix(glm::mat4&& transform);

	private:
		static glm::mat4 s_ViewProjectMatrix;
	};


	template<typename T, typename GLFunc>
	void Renderer::Draw(Shader& shader, const Ref<T>& obj, const glm::mat4& transform, GLFunc func)
	{
		auto& vao = obj->GetVertexArrayBuffer();
		vao->Bind();
		vao->GetIndexBuffer()->Bind();
		shader.Bind();
		shader.SetMat4("u_ProjectMatrix", s_ViewProjectMatrix);
		shader.SetMat4("u_ModelMatrix", transform);
		func();

	}
}