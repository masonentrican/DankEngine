#include "dankpch.h"

#include "Renderer2D.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dank
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> VertexArray;
		Ref<Shader> Shader;
	};

	static Renderer2DStorage* s_Data;


	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage;

		s_Data->VertexArray = VertexArray::Create();

		float vertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" }
		};

		vertexBuffer->SetLayout(layout);
		s_Data->VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		s_Data->VertexArray->SetIndexBuffer(indexBuffer);

		s_Data->Shader = Shader::Create("assets/shaders/FlatColor.glsl");		

	}

	void Renderer2D::ShutDown()
	{
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->Shader->Bind();
		s_Data->Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());	
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->Shader->Bind();
		s_Data->Shader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_Data->Shader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}
}