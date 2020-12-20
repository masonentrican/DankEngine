#include "dankpch.h"
#include "Application.h"

#include <glad/glad.h>
#include "Input.h"

namespace Dank {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		DANK_CORE_ASSERT(!s_Instance, "Application already exists.");
		s_Instance = this;

		_window = std::unique_ptr<Window>(Window::Create());
		_window->SetEventCallback(DANK_BIND_EVENT(Application::OnEvent));

		_imGuiLayer = new ImGuiLayer();
		PushOverlay(_imGuiLayer);


		// TEMPORARY
		// ------------------------------------------------------------------------
		// Building a triangle with openGL directly in the application for now.
		// This will need to be abstracted out
		//
		// Vertex Array - required for core OpenGL profile
		// Vertex Buffer
		// Index Buffer
		// ------------------------------------------------------------------------


		// ---------------------------------------------------
		// VERTEX BUFFER
		// ---------------------------------------------------

		// Generate the vertex array and bind it.
		glGenVertexArrays(1, &_vertexArray);
		glBindVertexArray(_vertexArray);

		// Generate the vertex buffer and bind it.
		glGenBuffers(1, &_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

		// Define the vertices for a triangle
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		// Upload the vertex data to the gpu
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Tell OpenGL the layout of the vertex data		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);




		// ---------------------------------------------------
		// INDEX BUFFER - OpenGL calls these Element Buffers
		// ---------------------------------------------------

		// Generate and bind the index buffer
		glGenBuffers(1, &_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

		// Declare and define the indices
		unsigned int indices[3] = { 0, 1, 2 };

		// Upload the index data to the gpu
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	}

	Application::~Application() {}

	void Application::PushLayer(Layer* layer)
	{
		_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		_layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		// Handle window close event through dispatch
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(DANK_BIND_EVENT(Application::OnWindowClose));

		// Iterate backwards through the layerStack to handle events that are at the end in overlays
		// before we handle events in the layers underneath it.
		for (auto it = _layerStack.end(); it != _layerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	//------------------------------------------//
	//			Main application loop			//
	//------------------------------------------//
	void Application::Run() {


		while (_running)
		{
			// Change window color to ensure glfw is working
			glClearColor(0.075f, 0.085f, 0.085f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// TEMPORARY - direct OpenGL impl
			glBindVertexArray(_vertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			// Run the OnUpdate for every layer in the stack
			for (Layer* layer : _layerStack)
				layer->OnUpdate();

			// ImGui layer rendering
			_imGuiLayer->Begin();
			for (Layer* layer : _layerStack)
				layer->OnImGuiRender();
			_imGuiLayer->End();

			// Update the window
			_window->OnUpdate();

		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}
}

