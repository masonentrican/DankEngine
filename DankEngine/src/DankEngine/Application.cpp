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

		// Generate the vertex array and bind it.
		glGenVertexArrays(1, &_vertexArray);
		glBindVertexArray(_vertexArray);

		// Define the vertices for a triangle
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		// Create Vertex Buffer
		_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		// Tell OpenGL the layout of the vertex data		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		// Declare and define the indices
		unsigned int indices[3] = { 0, 1, 2 };
		
		// Create Index Buffer
		_indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));



		// -----------------------------------------------------------------------------
		// VERTEX & FRAG SHADERS - Inline declaration and definition of shader source
		// ------------------------------------------------------------------------------

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}		
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.8, 0.1, 0.4, 1.0);
			}		
		)";

		_shader.reset(new Shader(vertexSrc, fragmentSrc));






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
			_shader->Bind();
			glBindVertexArray(_vertexArray);
			glDrawElements(GL_TRIANGLES, _indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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

