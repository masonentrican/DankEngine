#include "dankpch.h"
#include "TextureOpenGl.h"
#include "stb_image.h"
#include <glad/glad.h>
#include "Shader.h"
#include "DankEngine/Renderer/Renderer.h"

namespace Dank {
	TextureOpenGl::TextureOpenGl(const std::string& filepath)
	{		

		unsigned int texture;
		unsigned char* data;

		stbi_set_flip_vertically_on_load(true); 
		data = stbi_load(filepath.c_str(), &_width, &_height, &_numColorChannels, 0);
		DANK_CORE_ASSERT(data, "Failed to load image");

		// Dynamic Color depending on num of color channels
		GLenum colorFormat = 0, dataFormat = 0;
		if (_numColorChannels == 3)
		{
			colorFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}			
		else if (_numColorChannels == 4)
		{
			colorFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
			

		// Todo: move to renderer maybe an init?
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glGenTextures(1, &texture);
		//glBindTexture(GL_TEXTURE_2D, texture);

		glCreateTextures(GL_TEXTURE_2D, 1, &_iD);
		glTextureStorage2D(_iD, 1, colorFormat, _width, _height);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// gen
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
		glTextureSubImage2D(_iD, 0, 0, 0, _width, _height, dataFormat, GL_UNSIGNED_BYTE, data);		



		
		

		
		//glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		
		//_iD = texture;

	}
	
	TextureOpenGl::~TextureOpenGl()
	{}
	void TextureOpenGl::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, _iD);
	}
}