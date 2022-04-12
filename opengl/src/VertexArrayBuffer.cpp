
#include "VertexArrayBuffer.h"

#include<glad/glad.h>
namespace OGL
{
	
	VertexArrayBuffer::VertexArrayBuffer()
	{
		glGenVertexArrays(1, &m_RendererId);
	}

	VertexArrayBuffer::~VertexArrayBuffer()
	{
		glDeleteVertexArrays(1, &m_RendererId);
	}

	void VertexArrayBuffer::Bind()const
	{
		glBindVertexArray(m_RendererId);
	}

	void VertexArrayBuffer::UnBind()const
	{
		glBindVertexArray(0);
	}

	void VertexArrayBuffer::SetVertexBuffer(Ref<VertexBuffer>& vb, bool instanced)
	{
		auto TypeCast = [](OpenGLDataType type)->GLenum {
			switch (type)
			{
			case OGL::OpenGLDataType::Float: return GL_FLOAT;
				break;
			case OGL::OpenGLDataType::Int: return GL_INT;
				break;
			case OGL::OpenGLDataType::Double: return GL_DOUBLE;
				break;
			case OGL::OpenGLDataType::Byte: return GL_BYTE;
				break;
			case OGL::OpenGLDataType::UInt32: return GL_UNSIGNED_INT;
			}
			assert(false);
		};

		Bind();
		int offset = 0;
		for (auto vbo : m_VertexBuffers) offset += vbo->GetLayout().GetElements().size();
		m_VertexBuffers.push_back(vb);
		vb->Bind();
		auto& layout = vb->GetLayout();
		auto& LayoutElements = layout.GetElements();
		uint32_t totalSize = layout.GetTotalSize();
		
		for (int i = 0; i < LayoutElements.size(); i++)
		{
			const LayoutElement& e = LayoutElements[i];
			glVertexAttribPointer(i+offset, e.Count, TypeCast(e.Type), (e.Normalized == true) ? GL_TRUE : GL_FALSE, totalSize, (void*)e.offset);
			glEnableVertexAttribArray(i + offset);
			
			if (instanced) glVertexAttribDivisor(i + offset, e.Divisor_);
		}

		
		UnBind();
	}

	void VertexArrayBuffer::SetIndexBuffer(Ref<IndexBuffer>& ib)
	{
		Bind();
		ib->Bind();
		m_IndexBuffer = ib;
		UnBind();
	}

	Ref<VertexArrayBuffer> VertexArrayBuffer::Create()
	{
		return std::make_shared<VertexArrayBuffer>();
	}
}

