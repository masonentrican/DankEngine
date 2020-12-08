#include "dankpch.h"
#include "Application.h"

// Temp include glfw here to test window and color
#include <GLFW/glfw3.h>

namespace Dank {

	Application::Application()
	{
		_window = std::unique_ptr<Window>(Window::Create());
		_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {}

	void Application::PushLayer(Layer* layer)
	{
		_layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		_layerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		// Handle window close event through dispatch
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		// Iterate backwards through the layerStack to handle events that are at the end in overlays
		// before we handle events in the layers underneath it.
		for (auto it = _layerStack.end(); it != _layerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	//------------------------------------------//
	//			Main application loop			//
	//------------------------------------------//
	void Application::Run() {


		while (_running)
		{
			// Change window color to ensure glfw is working
			glClearColor(0, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// Run the OnUpdate for every layer in the stack
			for (Layer* layer : _layerStack)
				layer->OnUpdate();

			// Update the window
			_window->OnUpdate();

		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}
}

