#include "dankpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Dank
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}