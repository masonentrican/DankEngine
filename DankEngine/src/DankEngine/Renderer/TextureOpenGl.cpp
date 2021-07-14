#include "dankpch.h"
#include "TextureOpenGl.h"
#include "stb_image.h"
#include <glad/glad.h>
#include "Shader.h"
#include "DankEngine/Renderer/Renderer.h"

namespace Dank {
	TextureOpenGl::TextureOpenGl(const std::string& filepath)
	{
		const char* cFilePath = filepath.c_str();
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		int width, height, nrChannels;
		unsigned char* data = stbi_load(cFilePath, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
		
		_iD = texture;
		_width = width;
		_height = height;
		_numColorChannels = nrChannels;

	}
	
	TextureOpenGl::~TextureOpenGl()
	{}
	void TextureOpenGl::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, _iD);
	}
}