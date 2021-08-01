#include<dankpch.h>
#include "Mesh.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Dank {

	//Ref<Mesh> Mesh::Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	//{
	//	return std::make_shared<Mesh>(vertices, indices, textures);
	//	
	//}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures) : _vertices(vertices), _indices(indices), _textures(textures)
	{
		//setupMesh();
	}
	/*
	void Mesh::setupMesh()
	{
		//create and bind vertex array
		_vertexArray.reset(VertexArray::Create());
		_vertexArray->Bind();

		//create and bind vertex buffer
		_vertexBuffer.reset(VertexBuffer::Create((float*)&_vertices[0], _vertices.size() * sizeof(Vertex)));

		//create and bind index buffer
		_indexBuffer.reset(IndexBuffer::Create(&_indices[0], _indices.size()));


		//set vertex attributes and stride
		_vertexBuffer->SetVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		_vertexBuffer->SetVertexAttribute(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		_vertexBuffer->SetVertexAttribute(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		_vertexArray->Unbind();
		
	}
	
	void Mesh::Draw(Ref<Shader> shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;

		//std::cout << "textures: " << _textures.size() << std::endl;
			for (unsigned int i = 0; i < this->_textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);

				std::string number;
				std::string name = Mesh::_textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);
				else if (name == "texture_normal")
					number = std::to_string(normalNr++); // transfer unsigned int to stream
				
				std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt((name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, _textures[i].iD);

				
			}
			
			_vertexArray->Bind();
			//glBindVertexArray(VAO);
			//std::cout << "indices size: " << _indices.size() << " drawing" << "vertex array: " << VAO << std::endl;
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
			
			glActiveTexture(GL_TEXTURE0);

	}*/
}