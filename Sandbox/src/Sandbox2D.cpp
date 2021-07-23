#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _cameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	_texture = Dank::Texture2D::Create("assets/textures/weedleaf.png");
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
	
	Dank::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, glm::vec2(glm::sin(app.GetRunTime()) + 2.0f, 5.0f), _texture, 5.0f);	
	_texture->Unbind();

	Dank::Renderer2D::DrawQuad({ 1.0f, 0.2f, 0.1f }, { 0.75f, 0.5f }, { 0.2f, 0.7f, 0.3f, 1.0f });
	Dank::Renderer2D::DrawQuad({ 0.2f, -0.4f, 0.2f }, { 0.45f, 0.45f }, { 0.7f, 0.2f, 0.3f, 1.0f });
	Dank::Renderer2D::DrawRotatedQuad({ -0.75f, -0.1f, 0.3f }, { 0.4f, 0.75f }, 45.0f, { 0.2f, 0.3f, 0.7f, 1.0f });

	Dank::Renderer2D::EndScene();
	// -----------  END SCENE  -------------//
}

void Sandbox2D::OnEvent(Dank::Event& e)
{
	_cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
}