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
		float vertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		// Define basic indicies for a square
		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

		// Instantiate the vertex buffer and pass in the vertex data
		Dank::Ref<Dank::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Dank::VertexBuffer::Create(vertices, sizeof(vertices)));

		// Define and set the Vertex Buffer Layout
		Dank::BufferLayout layout = {
			{ Dank::ShaderDataType::Float3, "a_Position" }
		};
		vertexBuffer->SetLayout(layout);

		// Pass the vertex buffer reference to vertex array
		_vertexArray->AddVertexBuffer(vertexBuffer);

		// Declare an define the index buffer reference. Use it to set the index buffer
		Dank::Ref<Dank::IndexBuffer> indexBuffer;
		indexBuffer.reset(Dank::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));		
		_vertexArray->SetIndexBuffer(indexBuffer);

		// Create the shader
		_defaultShader = Dank::Shader::Create("assets/shaders/Default.glsl");

	}

	void OnUpdate(Dank::Timestep ts) override
	{
		float deltaTime = ts;

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

		std::dynamic_pointer_cast<Dank::OpenGLShader>(_defaultShader)->Bind();
		std::dynamic_pointer_cast<Dank::OpenGLShader>(_defaultShader)->UploadUniformFloat3("u_Color", _flatShaderColor);

		for (int y = -5; y < 5; y++)
		{
			for (int x = -5; x < 5; x++)
			{
				glm::vec3 pos(x * 0.15f, y * 0.15f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Dank::Renderer::Submit(_defaultShader, _vertexArray, transform);
			}
		}	

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
			ImGui::ColorEdit3("Shader Color", glm::value_ptr(_flatShaderColor));
			ImGui::End();
		}

	}

private:
	Dank::ShaderLibrary _shaderLibrary;

	Dank::Ref<Dank::Shader> _defaultShader;
	Dank::Ref<Dank::VertexArray> _vertexArray;

	Dank::OrthographicCamera _camera;

	glm::vec3 _cameraPosition;
	float _cameraRotation = 0.0f;

	float _cameraMoveSpeed = 2.0f;
	float _cameraRotationSpeed = 90.0f;

	glm::vec3 _flatShaderColor = { 0.5, 0.4, 0.4 };

	bool _isShaderSettingsOpen = true;

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