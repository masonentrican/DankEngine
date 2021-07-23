#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Dank
{
	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();
		
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// 2D Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
		

	};
}