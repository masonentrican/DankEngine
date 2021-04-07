#include "dankpch.h"
#include "Application.h"

#include "DankEngine/Renderer/Renderer.h"
#include "Input.h"

namespace Dank {

	Application* Application::s_Instance = nullptr;											// Create singleton instance of application

	Application::Application()																// Primary Application function
	{
		DANK_CORE_ASSERT(!s_Instance, "Application already exists.");
		s_Instance = this;

		_window = std::unique_ptr<Window>(Window::Create());								// Instantiate the window object
		_window->SetEventCallback(DANK_BIND_EVENT(Application::OnEvent));

		_imGuiLayer = new ImGuiLayer();														// Instantiate the ImGuiLayer and push it to the layer stack as an overlay
		PushOverlay(_imGuiLayer);
	}

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

			// Run the OnUpdate for every layer in the stack
			for (Layer* layer : _layerStack)
				layer->OnUpdate();

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
}

