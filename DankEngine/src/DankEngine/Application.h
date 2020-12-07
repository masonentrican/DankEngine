#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

// The base dank engine application.
namespace Dank {
	class DANK_API Application
	{
	public:
		Application();
		virtual~Application();

		void Run();

	private:
		std::unique_ptr<Window> _window;
		bool _running = true;
	};

	// Defined in the client
	Application* CreateApplication();
}
