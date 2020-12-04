#pragma once

#include "Core.h"

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
