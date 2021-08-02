#include "dankpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Dank {
	// OpenGL specific set clear color function
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(color.r, color.g, color.b, color.a);
	}

	// OpenGL specific clear color buffer function
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// OpenGL sepcific draw call using passed vertex array reference
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawArraysTriangles()
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void OpenGLRendererAPI::DrawModel(Ref<Model>& model)
	{
		Ref<Shader>& shader = model->GetShader();
		for (Mesh mesh : model->GetMeshes())
		{
			Ref<VertexArray>& vertexArray = VertexArray::Create();
			vertexArray->Bind();
			Ref<VertexBuffer>& vertexBuffer = VertexBuffer::Create((float*)&mesh._vertices[0], mesh._vertices.size() * sizeof(Vertex));
			Ref<IndexBuffer>& indexBuffer = IndexBuffer::Create(&mesh._indices[0], mesh._indices.size());
			vertexBuffer->SetVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			vertexBuffer->SetVertexAttribute(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
			vertexBuffer->SetVertexAttribute(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
			vertexArray->Unbind();
			//SubmitMesh(mesh, vertexArray);
			DrawMesh(std::make_shared<Mesh>(mesh), shader, vertexArray);
		}
	}

	void OpenGLRendererAPI::SubmitMesh(Mesh mesh, VertexArray *vertexArray)
	{
		//VertexArray *vertexArray = VertexArray::Create();
	
		//create and bind vertex array
		//vertexArray.reset(VertexArray::Create());
		//vertexArray->Bind();
		

		//set vertex attributes and stride
		//vertexBuffer->SetVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		//vertexBuffer->SetVertexAttribute(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		//vertexBuffer->SetVertexAttribute(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		//vertexArray->Unbind();
	}

	void OpenGLRendererAPI::DrawMesh(Ref<Mesh>& mesh, Ref<Shader> &shader, Ref<VertexArray>& vertexArray)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;

		//std::cout << "textures: " << _textures.size() << std::endl;
		for (unsigned int i = 0; i < mesh->_textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			std::string name = mesh->_textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream

			std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt((name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, mesh->_textures[i].iD);
			
		}
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("material.ambient", mesh->_material.Ambient);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("material.diffuse", mesh->_material.Diffuse);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("material.specular", mesh->_material.Specular);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("material.shininess", mesh->_material.Shininess);
		
		vertexArray->Bind();
		//glBindVertexArray(VAO);
		//std::cout << "indices size: " << _indices.size() << " drawing" << "vertex array: " << VAO << std::endl;
		glDrawElements(GL_TRIANGLES, mesh->_indices.size(), GL_UNSIGNED_INT, nullptr);
		vertexArray->Unbind();

		glActiveTexture(GL_TEXTURE0);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}
}