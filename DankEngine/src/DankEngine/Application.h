#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Dank {
	class DANK_API Application
	{
	public:
		Application();
		virtual~Application();

		void Run();
	};

	// Defined in the client
	Application* CreateApplication();
}
