#include "dankpch.h"
#include "Win_Window.h"

#include "DankEngine/Events/ApplicationEvent.h"
#include "DankEngine/Events/MouseEvent.h"
#include "DankEngine/Events/KeyEvent.h"

#include <glad/glad.h>



// Windows specific implementation of the Windows object. Defines
// the various attributes and interface functionality of a window on
// windows operating systems. Currently hard coded for GLFW

namespace Dank {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		DANK_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

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
		// Set window attributes
		_windowAttributes.Title  = windowProperties.Title;
		_windowAttributes.Width  = windowProperties.Width;
		_windowAttributes.Height = windowProperties.Height;

		// Log the attributes
		DANK_CORE_INFO("Creating window {0} ({1}, {2})", windowProperties.Title, windowProperties.Width, windowProperties.Height);

		// Init GLFW if it isn't already and handle errors
		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			DANK_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		// GLFW Init
		_window = glfwCreateWindow((int)windowProperties.Width, (int)windowProperties.Height, windowProperties.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(_window);

		// Glad Init
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DANK_CORE_ASSERT(status, "Failed to initialize Glad");

		// Set a pointer to the window and enable vsync for now
		glfwSetWindowUserPointer(_window, &_windowAttributes);
		SetVSync(true);

		// Set GLFW event callbacks
		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		{
			WindowAttributes& attributes = *(WindowAttributes*)glfwGetWindowUserPointer(window);
			attributes.Width = width;
			attributes.Height = height;
				
			WindowResizeEvent event(width, height);
			attributes.EventCallback(event);

		});

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
		{
			WindowAttributes& attributes = *(WindowAttributes*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			attributes.EventCallback(event);
		});

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mod)
		{
			WindowAttributes& attributes = *(WindowAttributes*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key,0);
					attributes.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					attributes.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					attributes.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keycode)
		{
				WindowAttributes& attributes = *(WindowAttributes*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				attributes.EventCallback(event);
		});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowAttributes& attributes = *(WindowAttributes*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				attributes.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				attributes.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowAttributes& attributes = *(WindowAttributes*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				attributes.EventCallback(event);
			});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowAttributes& attributes = *(WindowAttributes*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				attributes.EventCallback(event);
			});
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