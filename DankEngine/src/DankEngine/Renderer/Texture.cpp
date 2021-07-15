#include "dankpch.h"
#include "Texture.h"
#include "TextureOpenGl.h"
#include <glad/glad.h>
#include "Shader.h"
#include "Renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Dank {

	Texture* Texture::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    DANK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new TextureOpenGl(filepath); 
		}

		DANK_CORE_ASSERT(false, "Unknown RedererAPI");
		return nullptr;

	}


}