#pragma once
#include"Macro.h"

namespace OGL
{
	class Texture
	{
	public:
		Texture() = default;
		Texture(const std::string& filepath);
		~Texture();

		uint32_t GetRendererId() { return m_RendererId; }
		void Bind();
		void UnBind();
		static Ref<Texture> Create(const std::string& filepath)
		{
			return std::make_shared<Texture>(filepath);
		}
	private:
		uint32_t m_RendererId;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Channels;
	};
}