#pragma once

#include"VertexBuffer.h"
#include"IndexBuffer.h"
namespace OGL
{
	class VertexArrayBuffer
	{
	public:
		VertexArrayBuffer();
		~VertexArrayBuffer();

		void Bind() const;
		void UnBind() const;

		void SetVertexBuffer(Ref<VertexBuffer>& vb,bool instanced=false);
		void SetIndexBuffer(Ref<IndexBuffer>& ib);

		const Ref<IndexBuffer>& GetIndexBuffer()const { return m_IndexBuffer; }
		static Ref<VertexArrayBuffer> Create();
	private:
		uint32_t m_RendererId;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}