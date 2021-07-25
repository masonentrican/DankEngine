#include "dankpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace Dank {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: _width(width), _height(height)
	{
		DANK_PROFILE_FUNCTION();

		_internalFormat = GL_RGBA8;
		_dataFormat = GL_RGBA;		

		glCreateTextures(GL_TEXTURE_2D, 1, &_rendererId);
		glTextureStorage2D(_rendererId, 1, _internalFormat, _width, _height);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		// 
		// TODO: Why is this throwing errors? -  GL_INVALID_ENUM error generated. Invalid target.
		glTextureParameteri(_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
	{		
		DANK_PROFILE_FUNCTION();

		unsigned int texture;
		unsigned char* data;

		stbi_set_flip_vertically_on_load(true); 

		{
			DANK_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");

			data = stbi_load(filepath.c_str(), &_width, &_height, &_numColorChannels, 0);
		}
		
		DANK_CORE_ASSERT(data, "Failed to load image");

		// Dynamic color and data format depending on num of color channels
		GLenum internalFormat = 0, dataFormat = 0;
		if (_numColorChannels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}			
		else if (_numColorChannels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		_internalFormat = internalFormat;
		_dataFormat = dataFormat;
		
		glCreateTextures(GL_TEXTURE_2D, 1, &_rendererId);
		glTextureStorage2D(_rendererId, 1, internalFormat, _width, _height);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTextureParameteri(_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(_rendererId, 0, 0, 0, _width, _height, dataFormat, GL_UNSIGNED_BYTE, data);		

		stbi_image_free(data);

	}
	
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		DANK_PROFILE_FUNCTION();

		glDeleteTextures(1, &_rendererId);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		DANK_PROFILE_FUNCTION();

		// Check that the size of our texture data in bytes is the size of the actual texture.
		// Only support RGBA and RGB currently.
		uint32_t bytesPerPixel = _dataFormat == GL_RGBA ? 4 : 3;
		DANK_CORE_ASSERT(size == _width * _height * bytesPerPixel, "OpenGLTexture2D data must be for the entire texture");

		glTextureSubImage2D(_rendererId, 0, 0, 0, _width, _height, _dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, _rendererId);
	}
}