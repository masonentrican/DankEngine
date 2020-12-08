#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "DankEngine/Events/ApplicationEvent.h"



// The base dank engine application.
namespace Dank {
	class DANK_API Application
	{
	public:
		Application();
		virtual~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		std::unique_ptr<Window> _window;
		bool _running = true;

		bool OnWindowClose(WindowCloseEvent& e);

		LayerStack _layerStack;
	};

	// Defined in the client
	Application* CreateApplication();
}
