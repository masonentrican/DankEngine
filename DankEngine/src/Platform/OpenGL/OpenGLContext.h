#pragma once

#include "DankEngine/Renderer/GraphicsContext.h"

// Forward declaration so we dont need to include GLFW
struct GLFWwindow;

namespace Dank {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* _windowHandle;
	};

}
