#pragma once

#include<Macro.h>
#include<vector>
namespace OGL
{
	enum class OpenGLDataType
	{
		//�õ�ɶ��ɶ
		None=0,Float,Int,Double,Byte,UInt32
	};

	static uint32_t GetOpenGLDataSize(OpenGLDataType type)
	{
		switch (type)
		{
			case OpenGLDataType::Float: return 4;
			case OpenGLDataType::Int: return 4;
			case OpenGLDataType::UInt32: return 4;
			case OpenGLDataType::Double: return 8;
			case OpenGLDataType::Byte: return 1;
		}
		assert(false);
		return 0;
	}
	struct LayoutElement
	{
		std::string Name;
		OpenGLDataType Type;
		uint32_t Count;
		bool Normalized;
		
		uint32_t offset;
		LayoutElement(const std::string& name, OpenGLDataType type, uint32_t count, bool normalized = false)
			:Name(name),Type(type),Count(count),Normalized(normalized){}

		
	};

	class BufferLayout
	{
	public:
		BufferLayout(){}
		BufferLayout(const std::initializer_list<LayoutElement>& elements)
			:Elements(elements)
		{
			CalculateOffsetAndSize();
		}

		inline const std::vector<LayoutElement>& GetElements()const { return Elements; }
		inline uint32_t GetTotalSize()const { return m_TotalSize; }

 	private:
		inline void CalculateOffsetAndSize()
		{
			uint32_t offset = 0;
			m_TotalSize = 0;
			for (auto& element : Elements)
			{
				element.offset = offset;
				offset += element.Count * GetOpenGLDataSize(element.Type);
			}
			m_TotalSize = offset;
		}
	private:
		std::vector<LayoutElement> Elements;
		uint32_t m_TotalSize;
	};



	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, uint32_t size);
		~VertexBuffer();

		void Bind();
		void UnBind();

		inline void SetLayout(const BufferLayout& layout)
		{
			m_Layout = layout;
		}

		inline const BufferLayout& GetLayout()const { return m_Layout; }
		static Ref<VertexBuffer> Create(const void* data, uint32_t size);
	private:
		uint32_t m_RendererId;
		BufferLayout m_Layout;
	};



}