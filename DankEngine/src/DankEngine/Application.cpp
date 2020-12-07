#include "dankpch.h"
#include "Application.h"
#include "DankEngine/Log.h"

#include "DankEngine/Events/ApplicationEvent.h"

// Temp include glfw here to test window and color
#include <GLFW/glfw3.h>

namespace Dank {

	Application::Application()
	{
		_window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application() {}

	void Application::Run() {


		while (_running)
		{
			// Test glfw
			glClearColor(0, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			_window->OnUpdate();
		}

	}
}