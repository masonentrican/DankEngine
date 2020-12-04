#pragma once

#include "Core.h"

namespace DankEngine {

	class DANK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	};

	// Defined in client
	Application* CreateApplication();


}

