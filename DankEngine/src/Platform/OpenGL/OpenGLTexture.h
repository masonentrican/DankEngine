#pragma once
#include "DankEngine/Renderer/Texture.h"

namespace Dank {
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string& filepath);
		virtual ~OpenGLTexture();
		virtual void Bind() const override;

	private:
		int _width;
		int _height;
		int _numColorChannels;
		unsigned int _iD;

	};
}