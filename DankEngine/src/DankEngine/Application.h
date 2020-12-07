#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "DankEngine/Events/ApplicationEvent.h"

#include "Window.h"


// The base dank engine application.
namespace Dank {
	class DANK_API Application
	{
	public:
		Application();
		virtual~Application();

		void Run();

		void OnEvent(Event& e);


	private:
		std::unique_ptr<Window> _window;
		bool _running = true;

		bool OnWindowClose(WindowCloseEvent& e);
	};

	// Defined in the client
	Application* CreateApplication();
}
