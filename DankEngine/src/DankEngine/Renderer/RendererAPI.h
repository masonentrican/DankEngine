#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"
#include "Model.h"


namespace Dank {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawArraysTriangles() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void DrawModel(Ref<Model>& model) = 0;
		virtual void DrawMesh(Ref<Mesh>& mesh, Ref<Shader>& shader, Ref<VertexArray>& vertexArray) = 0;
		virtual void SubmitMesh(Mesh mesh, VertexArray* vertexArray) = 0;
		inline static API GetAPI() { return s_API; }
		
	private:
		static API s_API;
	};

}