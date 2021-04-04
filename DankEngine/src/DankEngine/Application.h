#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "DankEngine/Events/ApplicationEvent.h"

#include "DankEngine/ImGui/ImGuiLayer.h"

#include "DankEngine/Renderer/Shader.h"
#include "DankEngine/Renderer/Buffer.h"
#include "DankEngine/Renderer/VertexArray.h"
#include "DankEngine/Renderer/OrthographicCamera.h"



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

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *_window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		bool _running = true;

		std::unique_ptr<Window> _window;
		ImGuiLayer* _imGuiLayer;
		LayerStack _layerStack;
	
		std::shared_ptr<Shader> _shader;
		std::shared_ptr<VertexArray> _vertexArray;

		std::shared_ptr<Shader> _blueShader;
		std::shared_ptr<VertexArray> _squareVA;

		OrthographicCamera _camera;

	private:
		static Application* s_Instance;
	};

	// Defined in the client
	Application* CreateApplication();
}
