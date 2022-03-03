
#include "FrameBuffer.h"

#include<glad/glad.h>
namespace OGL
{
	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
	{
		Init(width, height);
	}
	FrameBuffer::~FrameBuffer()
	{
		Destroy();
	}
	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
	}
	void FrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		Destroy();
		Init(width, height);
	}

	Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height)
	{
		return std::make_shared<FrameBuffer>(width, height);
	}

	void FrameBuffer::Init(uint32_t width, uint32_t height)
	{
		glGenFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		glGenTextures(1, &m_textureAttachment);
		glBindTexture(GL_TEXTURE_2D, m_textureAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureAttachment, 0);
		/*
		glGenRenderbuffers(1, &m_DepthAndStencilAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAndStencilAttachment);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAndStencilAttachment);
		*/
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) assert(false);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FrameBuffer::Destroy()
	{
		glDeleteFramebuffers(1, &m_RendererId);
		glDeleteTextures(1, &m_textureAttachment);
		//glDeleteRenderbuffers(1, &m_DepthAndStencilAttachment);
	}





	//MultiSampleFrameBuffer

	MultiSampleFrameBuffer::MultiSampleFrameBuffer(uint32_t width, uint32_t height)
		:m_IntermediateFbo(width,height),
		m_Width(width),m_Height(height)
	{
		Init(width,height);
	}

	MultiSampleFrameBuffer::~MultiSampleFrameBuffer()
	{
		Destroy();
	}

	void MultiSampleFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
	}

	void MultiSampleFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void MultiSampleFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
		m_IntermediateFbo.Resize(width, height);
		Destroy();
		Init(width, height);
	}

	Ref<MultiSampleFrameBuffer> MultiSampleFrameBuffer::Create(uint32_t width, uint32_t height)
	{
		return std::make_shared<MultiSampleFrameBuffer>(width, height);
	}

	void MultiSampleFrameBuffer::Blit()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_IntermediateFbo.m_RendererId);
		glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void MultiSampleFrameBuffer::Init(uint32_t width, uint32_t height)
	{
		glGenFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		glGenTextures(1, &m_MultiSampleTextureAttachment);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MultiSampleTextureAttachment);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_MultiSampleTextureAttachment, 0);

		glGenRenderbuffers(1, &m_DepthAndStencilAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAndStencilAttachment);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAndStencilAttachment);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) assert(false);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void MultiSampleFrameBuffer::Destroy()
	{
		glDeleteFramebuffers(1, &m_RendererId);
		glDeleteTextures(1, &m_MultiSampleTextureAttachment);
		glDeleteRenderbuffers(1, &m_DepthAndStencilAttachment);
	}
}
