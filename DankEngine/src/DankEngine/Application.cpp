#include "dankpch.h"
#include "Application.h"

#include "DankEngine/Renderer/Renderer.h"
#include "Input.h"

namespace Dank {

	Application* Application::s_Instance = nullptr;											// Create singleton instance of application

	Application::Application() : _camera(-1.6f, 1.6f, -0.9f, 0.9f)							// Primary Application function
	{
		DANK_CORE_ASSERT(!s_Instance, "Application already exists.");
		s_Instance = this;

		_window = std::unique_ptr<Window>(Window::Create());								// Instantiate the window object
		_window->SetEventCallback(DANK_BIND_EVENT(Application::OnEvent));

		_imGuiLayer = new ImGuiLayer();														// Instantiate the ImGuiLayer and push it to the layer stack as an overlay
		PushOverlay(_imGuiLayer);

		_vertexArray.reset(VertexArray::Create());											// Instatiate a vertex array

		float vertices[3 * 7] = {															// Define the vertex and color data for the vertex array
			-0.5f, -0.5f, 0.0f, 0.0f, 0.6f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.4f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.2f, 0.1f, 1.0f
		};
		
		std::shared_ptr<VertexBuffer> vertexBuffer;											// Instantiate the vertex buffer and pass in the vertex data
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {																// Define the Vertex Buffer Layout
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};


		vertexBuffer->SetLayout(layout);													// Set the layout to the vertex buffer instance
		_vertexArray->AddVertexBuffer(vertexBuffer);										// Pass the vertex buffer reference to vertex array

		uint32_t indices[3] = { 0, 1, 2 };													// Define basic indicies for a triangle

		std::shared_ptr<IndexBuffer> indexBuffer;											// Declare the index buffer pointer
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));// Define the index buffer values from the indices

		_vertexArray->SetIndexBuffer(indexBuffer);											// Pass the index array to the vertex array for reference


		_squareVA.reset(VertexArray::Create());												// Create the squareVA vertex array
		float squareVertices[3 * 4] = {														// Define the square verticies
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
	
		std::shared_ptr<VertexBuffer> squareVB;												// Declare the vertex buffer for the square
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));		// Instantiate the square vertex buffer

		BufferLayout squareLayout = {														// Define the Square Vertex Buffer Layout
			{ ShaderDataType::Float3, "a_Position" }
		};
		
		squareVB->SetLayout(squareLayout);													// Set the layout to the square vertex buffer instance
		_squareVA->AddVertexBuffer(squareVB);												// Pass the square vertex buffer instance to the square vertex array
		
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };									// Define the square indicies

		std::shared_ptr<IndexBuffer> squareIB;												// Declare the square index buffer pointer
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t))); // Define the values
		_squareVA->SetIndexBuffer(squareIB);												// Pass the square index array to the vertex array for ref


		// -----------------------------------------------------------------------------
		// VERTEX & FRAG SHADERS - Inline declaration and definition of shader source
		// ------------------------------------------------------------------------------

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			uniform mat4 u_ViewProjection;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}	
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
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
			uniform mat4 u_ViewProjection;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
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
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			_camera.SetPosition({0.0f, 0.0f, 0.0f});
			_camera.SetRotation( 0.0f );

			Renderer::BeginScene(_camera);

			Renderer::Submit(_blueShader, _squareVA);
			Renderer::Submit(_shader, _vertexArray);

			Renderer::EndScene();

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

