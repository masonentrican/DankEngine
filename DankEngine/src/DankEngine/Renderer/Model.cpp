#include "dankpch.h"
#include "Model.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include <../assimp/Importer.hpp>
#include <../assimp/scene.h>
#include <../assimp/postprocess.h>
#include <stb_image.h>


namespace Dank {

	Model::Model(std::string &path, Ref<Shader> &shader) : _shader(shader)
	{
		stbi_set_flip_vertically_on_load(true);
		loadModel(path);
	}

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


	void Model::Draw(Ref<Shader> shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			
			//meshes[i].Draw(shader);
			//TODO: FIX THIS SHIT
			//Renderer::DrawMesh(meshes[i], shader);
			break;
			
	}

	void Model::loadModel(std::string path)
	{

#ifdef DANK_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}

		directory = path.substr(0, path.find_last_of("/"));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
			//TODO: FIX THIS SHIT
			//meshes.emplace_back(std::make_shared<Mesh>(processMesh(mesh, scene)));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<MeshTexture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			//process vertex data
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);


			vertices.push_back(vertex);
		}

		//process indices

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//process material
		
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		
		
		return Mesh(vertices, indices, textures);
	}

	std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<MeshTexture> textures;
		
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					
					break;
				}
				
			}
			if (!skip)
			{
				
				MeshTexture texture;
				texture.iD = TextureFromFile(str.C_Str(), directory);
				texture.path = str.C_Str();
				texture.type = typeName;
				textures.push_back(texture);
				textures_loaded.push_back(texture);
				
				
			}
			
		}
		return textures;

	}


	unsigned int Model::TextureFromFile(const char* path, const std::string& directory)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

}

