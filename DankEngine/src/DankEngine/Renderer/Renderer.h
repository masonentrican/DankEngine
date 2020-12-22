#pragma once

namespace Dank {

	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return _rendererAPI; }
	private:
		static RendererAPI _rendererAPI;
	};

}