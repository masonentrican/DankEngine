#include "dankpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Dank {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: _specification(spec)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &_rendererID);
		glDeleteTextures(1, &_colorAttachment);
		glDeleteTextures(1, &_depthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (_rendererID)
		{
			glDeleteFramebuffers(1, &_rendererID);
			glDeleteTextures(1, &_colorAttachment);
			glDeleteTextures(1, &_depthAttachment);
		}

		glCreateFramebuffers(1, &_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, _rendererID);
		
		// Create Color Attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &_colorAttachment);
		glBindTexture(GL_TEXTURE_2D, _colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _specification.Width, _specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Assign
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorAttachment, 0);
		 
		// Create Depth Buffer Attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &_depthAttachment);
		glBindTexture(GL_TEXTURE_2D, _depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _specification.Width, _specification.Height);
		// Assign
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthAttachment, 0);

		DANK_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _rendererID);
		glViewport(0, 0, _specification.Width, _specification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		_specification.Width = width;
		_specification.Height = height;
		Invalidate();
	}


}