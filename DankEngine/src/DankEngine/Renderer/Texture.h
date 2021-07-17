#pragma once
#include "DankEngine/Core/Core.h"

namespace Dank {
	class Texture
	{
	public:
		virtual ~Texture() = default;
		static Ref<Texture> Create(const std::string& filepath);
		virtual void Bind() const = 0;
	};
}