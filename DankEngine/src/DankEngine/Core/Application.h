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
		Application(const std::string& name = "Dank App");
		virtual~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *_window; }
		inline float GetRunTime() { return _runTime; }
		inline float GetFps() { return _fps; }
		inline uint32_t GetFrameCount() { return _frameCount; }

		ImGuiLayer* GetImGuiLayer() { return _imGuiLayer; }

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
		float _fps = 0.0f;
		uint32_t _frameCount = 0;

	private:
		static Application* s_Instance;		
	};

	// Defined in the client
	Application* CreateApplication();
}