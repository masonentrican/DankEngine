#include "dankpch.h"
#include "Application.h"

#include "DankEngine/Core/Log.h"

#include "DankEngine/Renderer/Renderer.h"

#include "Input.h"

// TODO: Only needed due to time right now
#include <GLFW/glfw3.h>

namespace Dank {

	// Create singleton instance of application
	Application* Application::s_Instance = nullptr;											

	// Primary Application
	Application::Application()																
	{
		// Create the application instance
		DANK_CORE_ASSERT(!s_Instance, "Application already exists.");
		s_Instance = this;

		// Instantiate the window object
		_window = std::unique_ptr<Window>(Window::Create());								
		_window->SetEventCallback(DANK_BIND_EVENT(Application::OnEvent));
		_window->SetVSync(false);

		Renderer::Init();

		// Instantiate the ImGuiLayer and push it to the layer stack as an overlay
		_imGuiLayer = new ImGuiLayer();														
		PushOverlay(_imGuiLayer);		
	}

	// Deconstructor
	Application::~Application() {}

	void Application::PushLayer(Layer* layer)
	{
		_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		_layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		// Handle window close event through dispatch
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(DANK_BIND_EVENT(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(DANK_BIND_EVENT(Application::OnWindowResize));

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
			// TODO: Should be platform specific. ie Platform::GetTime()
			float time = (float)glfwGetTime();
			_runTime = time;

			Timestep timestep = time - _lastFrameTime;
			_lastFrameTime = time;
			

			// Run the OnUpdate for every layer in the stack
			// Dont run if we're minimized thats scuffed.
			if (!_minimized)
			{
				for (Layer* layer : _layerStack)
					layer->OnUpdate(timestep);
			}
			

			// ImGui layer rendering
			_imGuiLayer->Begin();
			for (Layer* layer : _layerStack)
				layer->OnImGuiRender();
			_imGuiLayer->End();

			// Update the window
			_window->OnUpdate();

		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			_minimized = true;
			return false;
		}

		_minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}

