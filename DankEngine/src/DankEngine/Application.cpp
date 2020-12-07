#include "dankpch.h"
#include "Application.h"
#include "DankEngine/Log.h"

// Needed for testing of events
#include "DankEngine/Events/ApplicationEvent.h"
#include "DankEngine/Events/KeyEvent.h"
#include "DankEngine/Events/MouseEvent.h"

namespace Dank {

	Application::Application() {}
	Application::~Application() {}

	void Application::Run() {		
		// Test all event classes
		WindowResizeEvent winResizeEvent(1280, 720);
		KeyPressedEvent keyPressedEvent(3,0);
		KeyReleasedEvent keyReleasedEvent(69);
		MouseButtonPressedEvent mouseButtonPressedEvent(1);
		MouseButtonReleasedEvent mouseButtonReleasedEvent(2);
		MouseScrolledEvent mouseScrolledEvent(50,0);


		if (winResizeEvent.IsInCategory(EventCategoryApplication)) {
			DANK_TRACE(winResizeEvent);
			DANK_WARN(keyPressedEvent);
			DANK_WARN(keyReleasedEvent);
			DANK_INFO(mouseButtonPressedEvent);
			DANK_INFO(mouseButtonReleasedEvent);
			DANK_INFO(mouseScrolledEvent);
		}

		while (true);
	}
}