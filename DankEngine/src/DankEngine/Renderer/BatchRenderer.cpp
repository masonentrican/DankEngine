#include "dankpch.h"
#include "BatchRenderer.h"
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <algorithm>

namespace Dank {

	struct Vertex3D
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		float texIndex;
		float texturesLoaded;
		glm::vec3 color;
		glm::mat4 model;

	};

	int vertexCount = 0;
	struct BatchData
	{
		//static const uint32_t MaxVertices = 409600;
		static const uint32_t MaxVertices = 409600;
		static const uint32_t MaxIndices = MaxVertices * 1.5f;
		static const uint32_t MaxTextureSlots = 32;

		uint32_t NumUsedVertices = 0;
		uint32_t NumUsedIndices = 0;
		uint32_t IndiceOffset = 0;
		uint32_t TextureSlotIndex = 0;
		

		std::array<Ref<MeshTexture>, MaxTextureSlots> TextureSlots;
		
		Ref<VertexArray>	ObjectVertexArray;
		Ref<VertexBuffer>	ObjectVertexBuffer;
		Ref<Shader>			DefaultShader;


		Vertex3D* ObjectVertexBufferBase = nullptr;
		Vertex3D* ObjectVertexBufferPtr = nullptr;

		std::vector<uint32_t> ObjectIndexBuffer;

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
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TexturesLoaded" },
			{ ShaderDataType::Float3, "a_Color" },
			{ ShaderDataType::Float4, "a_Model1"},
			{ ShaderDataType::Float4, "a_Model2"},
			{ ShaderDataType::Float4, "a_Model3"},
			{ ShaderDataType::Float4, "a_Model4"}
			});
		s_Data.ObjectVertexArray->Unbind();

		s_Data.ObjectVertexArray->AddVertexBuffer(s_Data.ObjectVertexBuffer);
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;
		s_Data.ObjectVertexBufferBase = new Vertex3D[s_Data.MaxVertices];
		s_Data.DefaultShader = Shader::Create("assets/shaders/object.glsl");
		s_Data.DefaultShader->Bind();
		s_Data.DefaultShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
		s_Data.DefaultShader->Unbind();
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
		Ref<IndexBuffer> objectIndexBuffer = IndexBuffer::Create(&s_Data.ObjectIndexBuffer[0], s_Data.ObjectIndexBuffer.size());
		s_Data.ObjectVertexArray->SetIndexBuffer(objectIndexBuffer);
		Flush();
	}

	void BatchRenderer::Flush()
	{
		s_Data.DefaultShader->Bind();
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			glBindTextureUnit(i, s_Data.TextureSlots[i]->iD);
		}
		s_Data.ObjectVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.ObjectVertexArray, s_Data.NumUsedIndices);
		s_Data.ObjectVertexArray->Unbind();
		s_Data.stats.DrawCalls++;
		s_Data.ObjectIndexBuffer.clear();
		s_Data.IndiceOffset = 0;
		s_Data.NumUsedVertices = 0;
	}

	void BatchRenderer::FlushAndReset()
	{
		EndScene(); 
		s_Data.NumUsedVertices = 0;
		s_Data.NumUsedIndices = 0;
		s_Data.TextureSlotIndex = 0;
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


	void BatchRenderer::SubmitObject(std::vector<float>& vertices, std::vector<unsigned int>& indices , glm::vec3 position, float size, glm::vec3 color)
	{
		if (s_Data.NumUsedVertices + vertices.size() >= BatchData::MaxVertices)
			FlushAndReset();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size));
		s_Data.ObjectVertexArray->Bind();
		for (int i = 0; i < vertices.size(); i += 8)
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

		
		for (int i = 0; i < indices.size(); i++)
		{
			
			s_Data.ObjectIndexBuffer.emplace_back((s_Data.IndiceOffset) + indices[i]);
			
		}

		s_Data.IndiceOffset += (*max_element(indices.begin(), indices.end()) + 1);
		

		//s_Data.ObjectIndexBuffer.insert(s_Data.ObjectIndexBuffer.end(), indices.begin(), indices.end());

		s_Data.NumUsedIndices += indices.size();
		s_Data.NumUsedVertices += (vertices.size() / 8);
		s_Data.stats.ObjectCount++;


	}


	void BatchRenderer::SubmitObject(Ref<Model> Model, glm::vec3 position, float size)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size));
		s_Data.ObjectVertexArray->Bind();
		
		for (Mesh subMesh : Model->GetMeshes())
		{

			if (s_Data.NumUsedVertices + subMesh._vertices.size() >= BatchData::MaxVertices)
				FlushAndReset();


			float textureIndex = 0.0f;
			
			for (unsigned int i = 0; i < subMesh._textures.size(); i++)
			{
				bool alreadySlotted = false;
				
				for (uint32_t j = 0; j < s_Data.TextureSlotIndex; j++)
				{
					// This is ugly, basically just need to be able to compare rendererId's to see if we already have it slotted.
					if (s_Data.TextureSlots[j]->iD == subMesh._textures[i].iD)
					{
						textureIndex = (float)j;
						alreadySlotted = true;
						break;
					}
					//s_Data.TextureSlotIndex++;

				}

				if (!alreadySlotted)
				{
					
					textureIndex = (float)s_Data.TextureSlotIndex;
					s_Data.TextureSlots[s_Data.TextureSlotIndex] = CreateRef<MeshTexture>(subMesh._textures[i]);
					s_Data.TextureSlotIndex++;
					
					
				}
				
			}



			for (int i = 0; i < subMesh._vertices.size(); i++)
			{
				s_Data.ObjectVertexBufferPtr->position = subMesh._vertices[i].Position;
				s_Data.ObjectVertexBufferPtr->normal = subMesh._vertices[i].Normal;
				s_Data.ObjectVertexBufferPtr->texCoords = subMesh._vertices[i].TexCoords;
				s_Data.ObjectVertexBufferPtr->texIndex = textureIndex;
				if (subMesh._textures.size() == 0)
				{
					s_Data.ObjectVertexBufferPtr->texturesLoaded = 0.0f;
					s_Data.ObjectVertexBufferPtr->color = subMesh._material.Diffuse;
				}
				else
				{
					//fucking floating point precision. dont ask lol
					s_Data.ObjectVertexBufferPtr->texturesLoaded = 1.1f;
					s_Data.ObjectVertexBufferPtr->color = glm::vec3(0.0f);
				}
				s_Data.ObjectVertexBufferPtr->model = transform;
				s_Data.ObjectVertexBufferPtr++;
				
			}
			
			s_Data.ObjectVertexArray->Unbind();


			for (int i = 0; i < subMesh._indices.size(); i++)
			{

				s_Data.ObjectIndexBuffer.emplace_back((s_Data.IndiceOffset) + subMesh._indices[i]);

			}

			s_Data.IndiceOffset += (*max_element(subMesh._indices.begin(), subMesh._indices.end()) + 1);


			//s_Data.ObjectIndexBuffer.insert(s_Data.ObjectIndexBuffer.end(), indices.begin(), indices.end());

			s_Data.NumUsedIndices += subMesh._indices.size();
			s_Data.NumUsedVertices += (subMesh._vertices.size());
			s_Data.stats.VertexCount += subMesh._vertices.size();
			s_Data.stats.ObjectCount++;
		}
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