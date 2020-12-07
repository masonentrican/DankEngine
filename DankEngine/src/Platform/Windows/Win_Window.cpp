#include "dankpch.h"
#include "Win_Window.h"

// Windows specific implementation of the Windows object. Defines
// the various attributes and interface functionality of a window on
// windows operating systems. Currently hard coded for GLFW

namespace Dank {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProperties& windowProperties)
	{
		return new Win_Window(windowProperties);
	}

	Win_Window::Win_Window(const WindowProperties& windowProperties)
	{	
		Init(windowProperties);
	}

	Win_Window::~Win_Window()
	{
		Shutdown();
	}

	void Win_Window::Init(const WindowProperties& windowProperties)
	{
		
		_windowAttributes.Title  = windowProperties.Title;
		_windowAttributes.Width  = windowProperties.Width;
		_windowAttributes.Height = windowProperties.Height;

		DANK_CORE_INFO("Creating window {0} ({1}, {2})", windowProperties.Title, windowProperties.Width, windowProperties.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			DANK_CORE_ASSERT(success, "Could not intialize GLFW!");

			s_GLFWInitialized = true;
		}

		_window = glfwCreateWindow((int)windowProperties.Width, (int)windowProperties.Height, windowProperties.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(_window);
		glfwSetWindowUserPointer(_window, &_windowAttributes);
		SetVSync(true);
	}

	void Win_Window::Shutdown()
	{
		glfwDestroyWindow(_window);
	}

	void Win_Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(_window);
	}

	void Win_Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		_windowAttributes.VSync = enabled;
	}

	bool Win_Window::IsVSync() const {
		return _windowAttributes.VSync;
	}

}