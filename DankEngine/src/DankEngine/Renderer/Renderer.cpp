#include "dankpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Dank {

	Renderer::SceneData* Renderer::_sceneData = new Renderer::SceneData;

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		
		_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", _sceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Draw(const Ref<VertexArray>& vertexArray)
	{
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::DrawArraysTriangles()
	{
		RenderCommand::DrawArraysTriangles();
	}

	void Renderer::DrawModel(Ref<Model>& model)
	{
		RenderCommand::DrawModel(model);
	}

	void Renderer::DrawMesh(Ref<Mesh>& mesh, Ref<Shader>& shader, Ref<VertexArray>& vertexArray)
	{
		RenderCommand::DrawMesh(mesh, shader, vertexArray);
	}

	void Renderer::SubmitMesh(Mesh mesh, VertexArray* vertexArray)
	{
		RenderCommand::SubmitMesh(mesh, vertexArray);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

}