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
		Model(std::string &path, Ref<Shader> &shader);
		void Draw(Ref<Shader> shader);
		inline std::vector <Mesh> GetMeshes() { return meshes; }
		inline Ref<Shader>& GetShader() { return _shader; }
		inline void SetShader(Ref<Shader>& shader) { _shader = shader; }

	private:
		//TODO: Make this a vector of shared pointers
		std::vector <Mesh> meshes;
		std::string directory;
		std::vector <MeshTexture> textures_loaded;
		Ref<Shader> &_shader;
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene *scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		
		unsigned int TextureFromFile(const char* path, const std::string& directory);
		std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};

}
