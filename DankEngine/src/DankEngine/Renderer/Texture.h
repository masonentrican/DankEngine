#pragma once
#include "dankpch.h"
#include "DankEngine/Renderer/Renderer.h"

namespace Dank {
	class Texture
	{
	public:
		//Texture(const std::string& filepath);
		~Texture() = default;
		static Texture* Create(const std::string& filepath);
		virtual void Bind() const = 0;

	
	};
}