#include "dankpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Dank {
	// OpenGL specific set clear color function
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(color.r, color.g, color.b, color.a);
	}

	// OpenGL specific clear color buffer function
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// OpenGL sepcific draw call using passed vertex array reference
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawArraysTriangles()
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}
}