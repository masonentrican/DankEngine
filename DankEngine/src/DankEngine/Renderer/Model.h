#pragma once
#include "dankpch.h"
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include <../assimp/Importer.hpp>
#include <../assimp/scene.h>
#include <../assimp/postprocess.h>




namespace Dank {

	class Model
	{
	public:
		Model(std::string &path);
		void Draw(Ref<Shader> shader);
		
		

	private:
		//TODO: Make this a vector of shared pointers
		std::vector<Mesh> meshes;
		std::string directory;
		std::vector <Texture> textures_loaded;
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene *scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		unsigned int TextureFromFile(const char* path, const std::string& directory);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};

}
