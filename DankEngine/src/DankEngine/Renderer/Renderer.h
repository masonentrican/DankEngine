#pragma once

#include "RenderCommand.h"
#include "Model.h"
#include "OrthographicCamera.h"
#include "PerspectiveCameraController.h"
#include "Shader.h"

namespace Dank {

	class Renderer
	{
	public:		
		static void Init();

		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void Draw(const Ref<VertexArray>& vertexArray);
		static void DrawArraysTriangles();
		static void DrawModel(Ref<Model>& model);
		static void DrawMesh(Ref<Mesh>& mesh, Ref<Shader>& shader, Ref<VertexArray>& vertexArray);
		static void SubmitMesh(Mesh mesh, VertexArray* vertexArray);
		static void DrawPrimitive(Ref<VertexArray>& vertexArray, Ref<VertexBuffer>& vertexBuffer, glm::vec3 position, float size, glm::vec3 color, Ref<Shader>& shader);
		static Primitive CreatePrimitive(std::string type);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void OnWindowResize(uint32_t width, uint32_t height);

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* _sceneData;

	};

}