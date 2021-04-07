#include <Dank.h>
#include "ImGui/ImGui.h"

/*----------------------------------------------------------------------------------
The client side code. This is how you build a client executable using the Dank Engine
-------------------------------------------------------------------------------------*/

// Create a new layer
class TestLayer : public Dank::Layer
{
public:
	TestLayer() : Layer("Test"), _camera(-1.6f, 1.6f, -0.9f, 0.9f), _cameraPosition(0.0f)
	{
		// Instatiate a vertex array
		_vertexArray.reset(Dank::VertexArray::Create());											

		// Define the vertex and color data for the vertex array
		float vertices[3 * 7] = {															
			-0.5f, -0.5f, 0.0f, 0.0f, 0.6f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.4f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.2f, 0.1f, 1.0f
		};

		// Instantiate the vertex buffer and pass in the vertex data
		std::shared_ptr<Dank::VertexBuffer> vertexBuffer;											
		vertexBuffer.reset(Dank::VertexBuffer::Create(vertices, sizeof(vertices)));

		// Define the Vertex Buffer Layout
		Dank::BufferLayout layout = {																
			{ Dank::ShaderDataType::Float3, "a_Position" },
			{ Dank::ShaderDataType::Float4, "a_Color" }
		};

		// Set the layout to the vertex buffer instance
		vertexBuffer->SetLayout(layout);	

		// Pass the vertex buffer reference to vertex array
		_vertexArray->AddVertexBuffer(vertexBuffer);										

		// Define basic indicies for a triangle
		uint32_t indices[3] = { 0, 1, 2 };													

		// Declare the index buffer pointer
		std::shared_ptr<Dank::IndexBuffer> indexBuffer;	

		// Define the index buffer values from the indices
		indexBuffer.reset(Dank::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// Pass the index array to the vertex array for reference
		_vertexArray->SetIndexBuffer(indexBuffer);											

		// Create the squareVA vertex array
		_squareVA.reset(Dank::VertexArray::Create());

		// Define the square verticies
		float squareVertices[3 * 4] = {														
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		// Declare the vertex buffer for the square
		std::shared_ptr<Dank::VertexBuffer> squareVB;												

		// Instantiate the square vertex buffer
		squareVB.reset(Dank::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));		

		// Define the Square Vertex Buffer Layout
		Dank::BufferLayout squareLayout = {														
			{ Dank::ShaderDataType::Float3, "a_Position" }
		};

		// Set the layout to the square vertex buffer instance
		squareVB->SetLayout(squareLayout);	

		// Pass the square vertex buffer instance to the square vertex array
		_squareVA->AddVertexBuffer(squareVB);												

		// Define the square indicies
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };									

		// Declare the square index buffer pointer
		std::shared_ptr<Dank::IndexBuffer> squareIB;

		// Define the values
		squareIB.reset(Dank::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t))); 

		// Pass the square index array to the vertex array for ref
		_squareVA->SetIndexBuffer(squareIB);												


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

		_shader.reset(new Dank::Shader(vertexSrc, fragmentSrc));



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

		_blueShader.reset(new Dank::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}
	
	void OnUpdate() override
	{
		if (Dank::Input::IsKeyPressed(DANK_KEY_LEFT))
			_cameraPosition.x -= _cameraMoveSpeed;
		else if (Dank::Input::IsKeyPressed(DANK_KEY_RIGHT))
			_cameraPosition.x += _cameraMoveSpeed;

		if (Dank::Input::IsKeyPressed(DANK_KEY_UP))
			_cameraPosition.y += _cameraMoveSpeed;
		else if (Dank::Input::IsKeyPressed(DANK_KEY_DOWN))
			_cameraPosition.y -= _cameraMoveSpeed;

		if (Dank::Input::IsKeyPressed(DANK_KEY_A))
			_cameraRotation += _cameraRotationSpeed;
		else if (Dank::Input::IsKeyPressed(DANK_KEY_D))
			_cameraRotation -= _cameraRotationSpeed;

		Dank::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Dank::RenderCommand::Clear();

		_camera.SetPosition(_cameraPosition);
		_camera.SetRotation(_cameraRotation);

		Dank::Renderer::BeginScene(_camera);

		Dank::Renderer::Submit(_blueShader, _squareVA);
		Dank::Renderer::Submit(_shader, _vertexArray);

		Dank::Renderer::EndScene();
	}

	void OnEvent(Dank::Event& event) override
	{
		// Overall event logging
		// DANK_TRACE("{0}", event);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Window generated from Sandbox proj");
		ImGui::Text("This window was created from the sandbox project by using the ImGui obj.\n----------------------------------------\n\nImGui::Begin('Window title')\nImGui::Text('Window text')\nImGui::End()");
		ImGui::End();
	}

private:
	std::shared_ptr<Dank::Shader> _shader;
	std::shared_ptr<Dank::VertexArray> _vertexArray;

	std::shared_ptr<Dank::Shader> _blueShader;
	std::shared_ptr<Dank::VertexArray> _squareVA;

	Dank::OrthographicCamera _camera;

	glm::vec3 _cameraPosition;
	float _cameraRotation = 0.0f;

	float _cameraMoveSpeed	   = 0.05f;
	float _cameraRotationSpeed = 2.0f;
	
};

// Declare the Sandbox Class
class Sandbox : public Dank::Application
{
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
	}

	~Sandbox() {}
};

// Declare and instantiate a Sandbox app from Dank Engine
Dank::Application* Dank::CreateApplication()
{
	return new Sandbox();
}