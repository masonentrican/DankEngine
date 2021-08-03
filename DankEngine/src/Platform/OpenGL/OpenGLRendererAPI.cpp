#include "dankpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Dank {

	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         DANK_CORE_FATAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       DANK_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          DANK_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: DANK_CORE_TRACE(message); return;
		}

		DANK_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init()
	{
#ifdef DANK_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		
		
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	// OpenGL specific clear color buffer function
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
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
		//std::cout << mesh->_material.Diffuse.x << " x " << mesh->_material.Diffuse.y << " y "  << mesh->_material.Diffuse.z << " z diffuse" << std::endl;
		vertexArray->Bind();
		//glBindVertexArray(VAO);
		//std::cout << "indices size: " << _indices.size() << " drawing" << "vertex array: " << VAO << std::endl;
		glDrawElements(GL_TRIANGLES, mesh->_indices.size(), GL_UNSIGNED_INT, nullptr);
		vertexArray->Unbind();

		glActiveTexture(GL_TEXTURE0);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}



	void OpenGLRendererAPI::DrawPrimitive(Ref<VertexArray>& vertexArray, Ref<VertexBuffer>& vertexBuffer, glm::vec3 position, float size, glm::vec3 color, Ref<Shader>& shader)
	{
		vertexArray->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(size));
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("model", model);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("color", color);
		DrawArraysTriangles();
		vertexArray->Unbind();
		
	}


	Primitive OpenGLRendererAPI::CreatePrimitive(std::string type)
	{
		if (type == "Cube")
		{
			Ref<VertexArray>& vertexArray = VertexArray::Create();
			vertexArray->Bind();
			Ref<VertexBuffer>& vertexBuffer = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
			vertexBuffer->SetVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			vertexBuffer->SetVertexAttribute(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			vertexArray->Unbind();
			return Primitive(vertexArray, vertexBuffer);
		}
	}


}