#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _cameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{

}
void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Dank::Timestep ts)
{
	_cameraController.OnUpdate(ts);

	Dank::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Dank::RenderCommand::Clear();

	// ----------- BEGIN SCENE -------------//
	Dank::Renderer2D::BeginScene(_cameraController.GetCamera());

	Dank::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f, 1.0f }, { 0.5f, 0.2f, 0.2f, 1.0f });

	Dank::Renderer2D::EndScene();
	// -----------  END SCENE  -------------//
}

void Sandbox2D::OnEvent(Dank::Event& e)
{
	_cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Shader settings", &_isShaderSettingsOpen);
	ImGui::ColorEdit3("Shader Color", glm::value_ptr(_shaderDefaultColor));
	ImGui::End();
}