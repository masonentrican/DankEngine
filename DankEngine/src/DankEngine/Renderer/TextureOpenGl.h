#pragma once
#include "DankEngine/Renderer/Texture.h"

namespace Dank {
	class TextureOpenGl : public Texture
	{
	public:
		TextureOpenGl(const std::string& filepath);
		virtual ~TextureOpenGl();
		virtual void Bind() const override;

	private:
		int _width;
		int _height;
		int _numColorChannels;
		unsigned int _iD;

	};
}