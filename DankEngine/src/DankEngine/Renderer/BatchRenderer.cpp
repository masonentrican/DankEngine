#include "dankpch.h"
#include "BatchRenderer.h"
#include "glm/glm.hpp"

namespace Dank {

	struct Vertex3D
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 color;
		glm::mat4 model;

	};

	int vertexCount = 0;
	struct BatchData
	{
		static const uint32_t MaxVertices = 409600;
		uint32_t NumUsedVertices = 0;

		Ref<VertexArray>	ObjectVertexArray;
		Ref<VertexBuffer>	ObjectVertexBuffer;
		Ref<Shader>			DefaultShader;

		Vertex3D* ObjectVertexBufferBase = nullptr;
		Vertex3D* ObjectVertexBufferPtr = nullptr;

		BatchRenderer::Statistics stats;

	};

	static BatchData s_Data;


	void BatchRenderer::Init()
	{
		s_Data.ObjectVertexArray = VertexArray::Create();
		s_Data.ObjectVertexArray->Bind();
		s_Data.ObjectVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(Vertex3D));
		s_Data.ObjectVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoords" },
			{ ShaderDataType::Float3, "a_Color" },
			{ ShaderDataType::Float4, "a_Model1"},
			{ ShaderDataType::Float4, "a_Model2"},
			{ ShaderDataType::Float4, "a_Model3"},
			{ ShaderDataType::Float4, "a_Model4"}
			});
		s_Data.ObjectVertexArray->Unbind();

		s_Data.ObjectVertexArray->AddVertexBuffer(s_Data.ObjectVertexBuffer);

		s_Data.ObjectVertexBufferBase = new Vertex3D[s_Data.MaxVertices];
		s_Data.DefaultShader = Shader::Create("assets/shaders/object.glsl");
		
	}

	Ref<Shader>& BatchRenderer::GetShader()
	{
		return s_Data.DefaultShader;
	}

	void BatchRenderer::BeginScene(const PerspectiveCamera& camera)
	{

		s_Data.DefaultShader->Bind();
		s_Data.DefaultShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data.NumUsedVertices = 0;
		s_Data.ObjectVertexBufferPtr = s_Data.ObjectVertexBufferBase;

	}

	void BatchRenderer::EndScene()
	{

		uint32_t dataSize = (uint8_t*)(s_Data.ObjectVertexBufferPtr) - (uint8_t*)s_Data.ObjectVertexBufferBase;
		s_Data.ObjectVertexBuffer->SetData(s_Data.ObjectVertexBufferBase, dataSize);
		Flush();
	}

	void BatchRenderer::Flush()
	{
		s_Data.DefaultShader->Bind();
		s_Data.ObjectVertexArray->Bind();
		s_Data.DefaultShader->SetInt("diffuseLoaded", 0);
		RenderCommand::DrawArraysTriangles(s_Data.NumUsedVertices);
		s_Data.ObjectVertexArray->Unbind();
		s_Data.stats.DrawCalls++;
	}

	void BatchRenderer::FlushAndReset()
	{
		EndScene(); 
		s_Data.NumUsedVertices = 0;
		s_Data.ObjectVertexBufferPtr = s_Data.ObjectVertexBufferBase;
	}

	void BatchRenderer::SubmitObject(std::vector<float>& vertices, glm::vec3 position, float size, glm::vec3 color)
	{
		if (s_Data.NumUsedVertices + vertices.size() >= BatchData::MaxVertices)
			FlushAndReset();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size));
		s_Data.ObjectVertexArray->Bind();
		for (int i = 0; i < vertices.size(); i+=8)
		{
			
			s_Data.ObjectVertexBufferPtr->position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
			s_Data.ObjectVertexBufferPtr->normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
			s_Data.ObjectVertexBufferPtr->texCoords = glm::vec2(vertices[i + 6], vertices[i + 7]);
			s_Data.ObjectVertexBufferPtr->color = color;
			s_Data.ObjectVertexBufferPtr->model = transform;
			
			s_Data.ObjectVertexBufferPtr++;
			vertexCount++;
		}
		
		s_Data.ObjectVertexArray->Unbind();

		
		s_Data.NumUsedVertices += (vertices.size() / 8);
		s_Data.stats.ObjectCount++;


	}

	void BatchRenderer::ResetStats()
	{
		memset(&s_Data.stats, 0, sizeof(Statistics));
	}

	BatchRenderer::Statistics BatchRenderer::GetStats()
	{
		return s_Data.stats;
	}
}