#pragma once
#include "DankEngine/Renderer/Texture.h"

namespace Dank {
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filepath);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return _width; }
		virtual uint32_t GetHeight() const override { return _height; }

		virtual void Bind() const override;

	private:
		int _width;
		int _height;
		int _numColorChannels;
		unsigned int _iD;

	};
}