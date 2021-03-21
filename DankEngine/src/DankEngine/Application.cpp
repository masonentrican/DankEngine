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


		_vertexArray.reset(VertexArray::Create());

		// Define the vertex and color data for a triangle
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.6f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.4f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.2f, 0.1f, 1.0f
		};

		// Instantiate the vertex buffer passing in the vertex data
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		// Create Vertex Buffer Layout
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		// Set the vertex buffer layout
		vertexBuffer->SetLayout(layout);

		// Pass the vertex buffer reference to vertex array
		_vertexArray->AddVertexBuffer(vertexBuffer);

		// Create the index buffer for the vertex array
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		_vertexArray->SetIndexBuffer(indexBuffer);

		// Create the squareVA vertex array
		_squareVA.reset(VertexArray::Create());

		// Define the square verticies
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		// Create the vertex buffer for the square
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		// Define the layout of the square vertex buffer
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		_squareVA->AddVertexBuffer(squareVB);

		// Define the square indicies
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		_squareVA->SetIndexBuffer(squareIB);


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



		// -----------------------------------------------------------------------------
		// Blue Box Vertex and Frag Shaders
		// ------------------------------------------------------------------------------
		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.3, 0.3, 0.3, 1.0);
			}
		)";

		_blueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));


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

			_blueShader->Bind();
			_squareVA->Bind();
			glDrawElements(GL_TRIANGLES, _squareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			_shader->Bind();
			_vertexArray->Bind();
			glDrawElements(GL_TRIANGLES, _vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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

