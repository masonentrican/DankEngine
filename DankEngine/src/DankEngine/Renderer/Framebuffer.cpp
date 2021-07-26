#include "dankpch.h"
#include "Framebuffer.h"

#include "DankEngine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Dank {
	/// <summary>
	/// 
	/// </summary>
	/// <param name="spec"></param>
	/// <returns></returns>
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    DANK_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}

		DANK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}