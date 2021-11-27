#pragma once

#include<CoreModel/Macro.h>
#include<CoreModel/Log.h>
namespace IGame
{
	class FrameBuffer
	{
		friend class MultiSampleFrameBuffer;
	public:
		FrameBuffer(uint32_t width, uint32_t height);
		~FrameBuffer();

		void Bind();
		void UnBind();

		void Resize(uint32_t width, uint32_t height);
		inline uint32_t GetTextureAttachment()const { return m_textureAttachment; }

		static Ref<FrameBuffer> Create(uint32_t width, uint32_t height);
	private:
		void Init(uint32_t width, uint32_t height);
		void Destroy();
	private:
		uint32_t m_RendererId;
		uint32_t m_textureAttachment;

		/*暂时不需要
		uint32_t m_DepthAndStencilAttachment;
		*/
	};

	class MultiSampleFrameBuffer
	{
	public:
		MultiSampleFrameBuffer(uint32_t width, uint32_t height);
		~MultiSampleFrameBuffer();

		void Bind();
		void UnBind();

		void Resize(uint32_t width, uint32_t height);
		
		inline uint32_t GetIntermediateTexture()const { return m_IntermediateFbo.m_textureAttachment; }
		static Ref<MultiSampleFrameBuffer> Create(uint32_t width, uint32_t height);

		void Blit();
	private:
		void Init(uint32_t width, uint32_t height);
		void Destroy();
	private:
		FrameBuffer m_IntermediateFbo;

		uint32_t m_RendererId;
		uint32_t m_MultiSampleTextureAttachment;
		uint32_t m_DepthAndStencilAttachment;
		uint32_t m_Width, m_Height;
	};
}