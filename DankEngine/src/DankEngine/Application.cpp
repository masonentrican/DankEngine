#include "dankpch.h"
#include "Application.h"

#include <glad/glad.h>
#include "Input.h"

namespace Dank {

	Application* Application::s_Instance = nullptr;

	// Convert shader data type to the open gl base type during buffer creation
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Dank::ShaderDataType::Float:    return GL_FLOAT;
			case Dank::ShaderDataType::Float2:   return GL_FLOAT;
			case Dank::ShaderDataType::Float3:   return GL_FLOAT;
			case Dank::ShaderDataType::Float4:   return GL_FLOAT;
			case Dank::ShaderDataType::Mat3:     return GL_FLOAT;
			case Dank::ShaderDataType::Mat4:     return GL_FLOAT;
			case Dank::ShaderDataType::Int:      return GL_INT;
			case Dank::ShaderDataType::Int2:     return GL_INT;
			case Dank::ShaderDataType::Int3:     return GL_INT;
			case Dank::ShaderDataType::Int4:     return GL_INT;
			case Dank::ShaderDataType::Bool:     return GL_BOOL;
		}

		DANK_CORE_ASSERT(false, "Unknown ShaderDataType in OpenGL base type conversion");
		return 0;
	}

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

		// Define the vertex data for a triangle
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.8f, 0.2f, 1.0f
		};

		_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			// Create Vertex Buffer Layout
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};

			_vertexBuffer->SetLayout(layout);
		}


		// Iterate through the buffer layout elements
		uint32_t index = 0;
		const auto& layout = _vertexBuffer->GetLayout();

		for (const auto& element : layout)
		{
			// Tell OpenGL the layout of the vertex data
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);

			index++;
		}

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
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}		
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
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

