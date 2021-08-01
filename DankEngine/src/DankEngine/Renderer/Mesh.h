#pragma once
//#include "Renderer.h"
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


struct MeshTexture {
	unsigned int iD;
	std::string type;
	std::string path;
};

struct Material {
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Ambient;
	float Shininess;
};

namespace Dank{
	class Mesh
	{
	public:
		std::vector<Vertex>       _vertices;
		std::vector<uint32_t>     _indices;
		std::vector<MeshTexture>      _textures;
	

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
		


	private:
	
		
	};

}