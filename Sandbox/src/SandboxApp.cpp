#include <Dank.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "ImGui/ImGui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ModelTest.h"
#include "DankEngine/Core/EntryPoint.h"



/*----------------------------------------------------------------------------------
The client side code. This is how you build a client executable using the Dank Engine
-------------------------------------------------------------------------------------*/

// Create a new layer
class TestLayer : public Dank::Layer
{
public:
	TestLayer() : Layer("Test"), _cameraController(45.0f, 1280.0f/720.0f)
	{
	
		

		// Instatiate a vertex array
		_vertexArray.reset(Dank::VertexArray::Create());
		
		// Define the vertex and color data for the vertex array
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
		uint32_t indices[] = {
			0, 1, 3,
			1, 2, 3

		};

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
		std::dynamic_pointer_cast<Dank::OpenGLShader>(defaultShader)->UploadUniformInt("u_Texture", 0);


	}

	void OnUpdate(Dank::Timestep ts) override
	{
		// FPS DEBUG
		// DANK_INFO("FPS - {0}", 1000/ ts.GetMilliseconds());
		
		float deltaTime = ts;
		_runTime += ts.GetSeconds();

		_cameraController.OnUpdate(ts.GetSeconds());

		


		Dank::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Dank::RenderCommand::Clear();

		// ----------- BEGIN SCENE -------------//
		Dank::Renderer::BeginScene(_cameraController.GetCamera());

		// Get the shader from the library
		_defaultShader = _shaderLibrary.Get("Default");
		

		std::dynamic_pointer_cast<Dank::OpenGLShader>(_defaultShader)->UploadUniformFloat3("u_Color", _shaderDefaultColor);
		//std::dynamic_pointer_cast<Dank::OpenGLShader>(_defaultShader)->UploadUniformMat4("u_Model", _model.UpdateModel(deltaTime));
		
		// Bind the smile texture and submit



		glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
		};


		
		/*for (int i = 0; i < 10; i++)
		{
			_tWeed->Bind();
			glm::mat4 cube = glm::mat4(1.0f);
			cube = glm::translate(cube, cubePositions[i]);
			float angle = 20.0f * i;
			cube = glm::rotate(cube, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			if(i == 3)
			{
				cube = glm::rotate(cube, deltaTime * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			}
			std::dynamic_pointer_cast<Dank::OpenGLShader>(_defaultShader)->UploadUniformMat4("u_Model", cube);
			Dank::Renderer::Submit(_defaultShader, _vertexArray);
			Dank::Renderer::DrawArraysTriangles();
		}*/

		//_tWall->Bind();
		//Dank::Renderer::Submit(_defaultShader, _vertexArray);
		//Dank::Renderer::DrawArraysTriangles();
		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f));
		for (int y = -5; y < 5; y++)
		{
			for (int x = -5; x < 5; x++)
			{
				_tSmile->Bind();
				glm::vec3 pos(x * 0.4f, y * 0.4f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), glm::vec3(std::clamp(glm::sin(_runTime/2.0f), -0.3f, 0.3f)));
				Dank::Renderer::Submit(_defaultShader, _vertexArray, transform);
				Dank::Renderer::DrawArraysTriangles();
			}
		}
		
		
		//Dank::Renderer::Submit(_defaultShader, _vertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(glm::sin(_runTime))));

		// Draw
		//Dank::Renderer::Draw(_vertexArray);
		

		Dank::Renderer::EndScene();
		// -----------  END SCENE  -------------//

	}

	void OnEvent(Dank::Event& e) override
	{
		// Overall event logging
		// DANK_TRACE("{0}", e);

		_cameraController.OnEvent(e);

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

	Dank::PerspectiveCameraController _cameraController;
	//Dank::Model _model;
	Dank::Ref<Dank::Texture> _tSmile;
	Dank::Ref<Dank::Texture> _tWall;
	Dank::Ref<Dank::Texture> _tWeed;

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
		PushLayer(new ModelLayer());
	}

	~Sandbox() {}
};

// Declare and instantiate a Sandbox app from Dank Engine
Dank::Application* Dank::CreateApplication()
{
	return new Sandbox();
}