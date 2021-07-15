#include <Dank.h>
#include "Platform/OpenGL/OpenGLShader.h"
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

		// Create the textures
		_tWall = Dank::Texture::Create("assets/textures/wall.jpg");
		_tSmile = Dank::Texture::Create("assets/textures/awesomeface.png");
		_tWeed = Dank::Texture::Create("assets/textures/weedleaf.png");

		// Create the shaders
		auto defaultShader = _shaderLibrary.Load("assets/shaders/Default.glsl");		

		// Bind the shader for the traingles and upload uniforms
		std::dynamic_pointer_cast<Dank::OpenGLShader>(defaultShader)->Bind();
		std::dynamic_pointer_cast<Dank::OpenGLShader>(defaultShader)->UploadUniformFloat3("u_Color", _shaderDefaultColor);
		std::dynamic_pointer_cast<Dank::OpenGLShader>(defaultShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Dank::Timestep ts) override
	{
		float deltaTime = ts;
		_runTime += ts.GetSeconds();

		DANK_INFO("FPS - {0}", 1000/ ts.GetMilliseconds());

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

		// Get the shader from the library
		_defaultShader = _shaderLibrary.Get("Default");
		
		// Bind the wall texture and submit	
		_tWall->Bind();
		Dank::Renderer::Submit(_defaultShader, _vertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(glm::cos(_runTime))));

		// Bind the smile texture and submit
		_tSmile->Bind();
		Dank::Renderer::Submit(_defaultShader, _vertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(glm::sin(_runTime))));

		// Draw
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
	Dank::ShaderLibrary _shaderLibrary;

	Dank::Ref<Dank::Shader> _defaultShader;
	Dank::Ref<Dank::VertexArray> _vertexArray;

	Dank::OrthographicCamera _camera;

	Dank::Ref<Dank::Texture> _tSmile;
	Dank::Ref<Dank::Texture> _tWall;
	Dank::Ref<Dank::Texture> _tWeed;

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