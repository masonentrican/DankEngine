#pragma once
#include "Renderer.h"
#include <dankpch.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
//#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
};


struct Texture {
	unsigned int iD;
	std::string type;
	std::string path;
};


namespace Dank{
	class Mesh
	{
	public:
		std::vector<Vertex>       _vertices;
		std::vector<uint32_t>     _indices;
		std::vector<Texture>      _textures;
		unsigned int VAO;
		void setupMesh();
		inline Ref<VertexArray> GetVertexArray() { return _vertexArray; }

		//static Ref<Mesh> Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures); 

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		void Draw(Ref<Shader> shader);


	private:
		unsigned int VBO, EBO;
		Ref<VertexArray> _vertexArray;
		Ref<VertexBuffer> _vertexBuffer;
		Ref<IndexBuffer> _indexBuffer;
		
	};

}