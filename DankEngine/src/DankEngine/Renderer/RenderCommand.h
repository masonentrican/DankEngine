#pragma once

#include "RendererAPI.h"

namespace Dank {
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawArraysTriangles()
		{
			s_RendererAPI->DrawArraysTriangles();
		}

		inline static void DrawModel(Ref<Model>& model)
		{
			s_RendererAPI->DrawModel(model);
		}

		inline static void DrawMesh(Ref<Mesh>& mesh, Ref<Shader>& shader, Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawMesh(mesh, shader, vertexArray);
		}

		inline static void SubmitMesh(Mesh mesh, VertexArray* vertexArray)
		{
			s_RendererAPI->SubmitMesh(mesh, vertexArray);
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}