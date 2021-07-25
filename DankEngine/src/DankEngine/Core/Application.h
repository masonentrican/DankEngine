#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "DankEngine/Events/Event.h"
#include "DankEngine/Events/ApplicationEvent.h"

#include "DankEngine/Core/Timestep.h"
#include "DankEngine/ImGui/ImGuiLayer.h"

// The base dank engine application.
namespace Dank {
	class Application
	{
	public:
		Application();
		virtual~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *_window; }
		inline float GetRunTime() { return _runTime; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		bool _running = true;
		bool _minimized = false;

		std::unique_ptr<Window> _window;
		ImGuiLayer* _imGuiLayer;
		LayerStack _layerStack;
		float _lastFrameTime = 0.0f;
		float _runTime = 0.0f;

	private:
		static Application* s_Instance;		
	};

	// Defined in the client
	Application* CreateApplication();
}
