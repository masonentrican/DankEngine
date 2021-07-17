#include "Sandbox2D.h"
#include "ImGui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _cameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	_vertexArray = Dank::VertexArray::Create();

	float vertices[3 * 5] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.5f, 1.0f
	};

	Dank::Ref<Dank::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(Dank::VertexBuffer::Create(vertices, sizeof(vertices)));

	Dank::BufferLayout layout = {
		{ Dank::ShaderDataType::Float3, "a_Position" },
		{ Dank::ShaderDataType::Float2, "a_TextureCoords" }
	};

	vertexBuffer->SetLayout(layout);
	_vertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	Dank::Ref<Dank::IndexBuffer> indexBuffer;
	indexBuffer.reset(Dank::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	_vertexArray->SetIndexBuffer(indexBuffer);

	auto defaultShader = _shaderLibrary.Load("assets/shaders/Default.glsl");
	_texture = Dank::Texture::Create("assets/textures/weedleaf.png");

	std::dynamic_pointer_cast<Dank::OpenGLShader>(defaultShader)->Bind();
	std::dynamic_pointer_cast<Dank::OpenGLShader>(defaultShader)->UploadUniformInt("u_Texture", 0);
}
void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Dank::Timestep ts)
{
	_cameraController.OnUpdate(ts);

	Dank::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Dank::RenderCommand::Clear();

	// ----------- BEGIN SCENE -------------//
	Dank::Renderer::BeginScene(_cameraController.GetCamera());

	_defaultShader = _shaderLibrary.Get("Default");
	std::dynamic_pointer_cast<Dank::OpenGLShader>(_defaultShader)->UploadUniformFloat3("u_Color", _shaderDefaultColor);

	_texture->Bind();
	Dank::Renderer::Submit(_defaultShader, _vertexArray);
	Dank::Renderer::Draw(_vertexArray);

	Dank::Renderer::EndScene();
	// -----------  END SCENE  -------------//
}

void Sandbox2D::OnEvent(Dank::Event& e)
{
	_cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Shader settings", &_isShaderSettingsOpen);
	ImGui::ColorEdit3("Shader Color", glm::value_ptr(_shaderDefaultColor));
	ImGui::End();
}