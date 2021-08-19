#pragma once
#include "dankpch.h"
#include "RenderCommand.h"
#include "PerspectiveCamera.h"
#include "VertexArray.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dank {
	class BatchRenderer
	{



	public:

		static void Init();
		static void SubmitObject(std::vector<float>& vertices, glm::vec3 position, float size, glm::vec3 color);
		static void SubmitObject(std::vector<float>& vertices, std::vector<unsigned int>& indices , glm::vec3 position, float size, glm::vec3 color);
		static void BeginScene(const PerspectiveCamera& camera);
		static Ref<Shader>& GetShader();
		static void EndScene();
		static void Flush();
		static void FlushAndReset();


		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t ObjectCount = 0;
			uint32_t VertexCount = 0;
		};

		static Statistics GetStats();
		static void ResetStats();
	};
}