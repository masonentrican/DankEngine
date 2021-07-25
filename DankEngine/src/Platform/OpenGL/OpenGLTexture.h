#pragma once
#include "DankEngine/Renderer/Texture.h"

#include <glad/glad.h>

namespace Dank {
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& filepath);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return _width; }
		virtual uint32_t GetHeight() const override { return _height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot) const override;
		virtual void Unbind(uint32_t slot = 0) const override;

	private:
		uint32_t _rendererId;

		int _width, _height, _numColorChannels;		
		GLenum _internalFormat, _dataFormat;
	};
}