#include "dankpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Dank {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
	{		

		unsigned int texture;
		unsigned char* data;

		stbi_set_flip_vertically_on_load(true); 
		data = stbi_load(filepath.c_str(), &_width, &_height, &_numColorChannels, 0);
		DANK_CORE_ASSERT(data, "Failed to load image");

		// Dynamic color and data format depending on num of color channels
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
		
		glCreateTextures(GL_TEXTURE_2D, 1, &_iD);
		glTextureStorage2D(_iD, 1, colorFormat, _width, _height);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(_iD, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(_iD, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(_iD, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(_iD, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(_iD, 0, 0, 0, _width, _height, dataFormat, GL_UNSIGNED_BYTE, data);		

		stbi_image_free(data);

	}
	
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &_iD);
	}

	void OpenGLTexture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, _iD);
	}
}