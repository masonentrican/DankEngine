#pragma once

#include "DankEngine/Renderer/RendererAPI.h"

namespace Dank {
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawArraysTriangles() override;
		virtual void DrawModel(Model model) override;
		virtual void DrawMesh(Mesh mesh, Ref<Shader>& shader, VertexArray* vertexArray) override;
		virtual void SubmitMesh(Mesh mesh, VertexArray* vertexArray) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	private:
	};
}