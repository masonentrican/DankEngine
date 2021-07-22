#include "dankpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Dank {



	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : _windowHandle(windowHandle)
	{
		DANK_CORE_ASSERT(windowHandle, "Window handle is null.");
	}

	void OpenGLContext::Init()
	{
		DANK_PROFILE_FUNCTION();

		glfwMakeContextCurrent(_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DANK_CORE_ASSERT(status, "Failed to initialize Glad");

		DANK_CORE_INFO("OpenGL Info: ");
		DANK_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		DANK_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		DANK_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		DANK_PROFILE_FUNCTION();

		glfwSwapBuffers(_windowHandle);
	}

}