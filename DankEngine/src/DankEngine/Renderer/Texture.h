#pragma once
#include "DankEngine/Core/Core.h"

namespace Dank {
	class Texture
	{
	public:
		virtual ~Texture() = default;

	    virtual void Bind() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& filepath);
	};
}