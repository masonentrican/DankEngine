#pragma once

#include "DankEngine/Renderer/Framebuffer.h"

namespace Dank {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return _colorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return _specification; }
	private:
		uint32_t _rendererID = 0;
		uint32_t _colorAttachment, _depthAttachment;
		FramebufferSpecification _specification;
	};

}