#include <Dank.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "DankEngine/Renderer/TextureOpenGl.h"
#include "ImGui/ImGui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		float vertices[3 * 5] = {
			//positions         //texture coords
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f, 0.5f, 1.0f 
		};

		// Instantiate the vertex buffer and pass in the vertex data
		Dank::Ref<Dank::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Dank::VertexBuffer::Create(vertices, sizeof(vertices)));

		// Define the Vertex Buffer Layout
		Dank::BufferLayout layout = {
			{ Dank::ShaderDataType::Float3, "a_Position" },
			{ Dank::ShaderDataType::Float2, "a_TextureCoords" }
		};

		// Set the layout to the vertex buffer instance
		vertexBuffer->SetLayout(layout);

		// Pass the vertex buffer reference to vertex array
		_vertexArray->AddVertexBuffer(vertexBuffer);

		// Define basic indicies for a triangle
		uint32_t indices[3] = { 0, 1, 2 };

		// Declare the index buffer pointer
		Dank::Ref<Dank::IndexBuffer> indexBuffer;

		// Define the index buffer values from the indices
		indexBuffer.reset(Dank::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// Pass the index array to the vertex array for reference
		_vertexArray->SetIndexBuffer(indexBuffer);

		// Create the squareVA vertex array
		_squareVA.reset(Dank::VertexArray::Create());

		// Define the square verticies
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		// Declare the vertex buffer for the square
		Dank::Ref<Dank::VertexBuffer> squareVB;

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
		Dank::Ref<Dank::IndexBuffer> squareIB;

		// Define the values
		squareIB.reset(Dank::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		// Pass the square index array to the vertex array for ref
		_squareVA->SetIndexBuffer(squareIB);


		// -----------------------------------------------------------------------------
		// Texture shader
		// ------------------------------------------------------------------------------

		std::string textureVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;			

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec2 v_TexCoord;
			
			void main()
			{	
				v_TexCoord = a_TexCoord;				
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);				
			}	
		)";

		std::string textureFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform sampler2D u_Texture;						
			uniform vec3 u_Color;

			in vec2 v_TexCoord;

			void main()
			{
				
				color = texture(u_Texture, v_TexCoord) * vec4(u_Color, 1.0);			
			}	
		)";

		_shader.reset(Dank::Shader::Create(textureVertexSrc, textureFragmentSrc));

		_tWall = Dank::Texture::Create("assets/textures/wall.jpg");
		_tSmile = Dank::Texture::Create("assets/textures/awesomeface.png");
		_tWeed = Dank::Texture::Create("assets/textures/weedleaf.png");

	}

	void OnUpdate(Dank::Timestep ts) override
	{
		float deltaTime = ts;
		_runTime += ts.GetSeconds();

		//DANK_INFO("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		if (Dank::Input::IsKeyPressed(DANK_KEY_LEFT))
			_cameraPosition.x -= _cameraMoveSpeed * deltaTime;
		else if (Dank::Input::IsKeyPressed(DANK_KEY_RIGHT))
			_cameraPosition.x += _cameraMoveSpeed * deltaTime;

		if (Dank::Input::IsKeyPressed(DANK_KEY_UP))
			_cameraPosition.y += _cameraMoveSpeed * deltaTime;
		else if (Dank::Input::IsKeyPressed(DANK_KEY_DOWN))
			_cameraPosition.y -= _cameraMoveSpeed * deltaTime;

		if (Dank::Input::IsKeyPressed(DANK_KEY_A))
			_cameraRotation += _cameraRotationSpeed * deltaTime;
		else if (Dank::Input::IsKeyPressed(DANK_KEY_D))
			_cameraRotation -= _cameraRotationSpeed * deltaTime;

		Dank::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Dank::RenderCommand::Clear();

		_camera.SetPosition(_cameraPosition);
		_camera.SetRotation(_cameraRotation);

		// ----------- BEGIN SCENE -------------//
		Dank::Renderer::BeginScene(_camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Dank::OpenGLShader>(_shader)->Bind();
		std::dynamic_pointer_cast<Dank::OpenGLShader>(_shader)->UploadUniformFloat3("u_Color", _shaderDefaultColor);
		std::dynamic_pointer_cast<Dank::OpenGLShader>(_shader)->UploadUniformInt("u_Texture", 0);

		_tWeed->Bind();
		Dank::Renderer::Submit(_shader, _vertexArray);
		_tWall->Bind();
		Dank::Renderer::Submit(_shader, _vertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(glm::cos(_runTime))));
		_tSmile->Bind();
		Dank::Renderer::Submit(_shader, _vertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(glm::sin(_runTime))));
		
		Dank::Renderer::Draw(_vertexArray);

		Dank::Renderer::EndScene();
		// -----------  END SCENE  -------------//

	}

	void OnEvent(Dank::Event& event) override
	{
		// Overall event logging
		// DANK_TRACE("{0}", event);
	}

	virtual void OnImGuiRender() override
	{
		if (_isShaderSettingsOpen)
		{
			ImGui::Begin("Shader settings", &_isShaderSettingsOpen);
			ImGui::ColorEdit3("Shader Color", glm::value_ptr(_shaderDefaultColor));
			ImGui::End();
		}

	}

private:
	Dank::Ref<Dank::Shader> _shader;
	Dank::Ref<Dank::VertexArray> _vertexArray;

	Dank::Ref<Dank::Shader> _flatColorShader;
	Dank::Ref<Dank::VertexArray> _squareVA;

	Dank::OrthographicCamera _camera;

	//TODO: make shared pointer
	//Dank::Ref<Dank::Texture> _myTexture;
	Dank::Texture* _myTexture;

	Dank::Texture* _tSmile;
	Dank::Texture* _tWall;
	Dank::Texture* _tWeed;

	glm::vec3 _cameraPosition;
	float _cameraRotation = 0.0f;

	float _cameraMoveSpeed = 2.0f;
	float _cameraRotationSpeed = 90.0f;

	glm::vec3 _shaderDefaultColor = { 1.0f, 1.0f, 1.0f };

	bool _isShaderSettingsOpen = true;

	float _runTime = 0;

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