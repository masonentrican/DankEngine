#pragma once
#include "dankpch.h"
#include "DankEngine/Renderer/Renderer.h"

namespace Dank {
	class Texture
	{
	public:
		virtual ~Texture() = default;
		static Ref<Texture> Create(const std::string& filepath);
		virtual void Bind() const = 0;
	};
}