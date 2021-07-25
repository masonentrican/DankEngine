#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _cameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	tex_weed = Dank::Texture2D::Create("assets/textures/weedleaf.png");
	tex_smile = Dank::Texture2D::Create("assets/textures/awesomeface.png");
}
void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Dank::Timestep ts)
{
	_cameraController.OnUpdate(ts);

	//DANK_TRACE("FPS: {0}", 1000 / ts.GetMilliseconds());

	Dank::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Dank::RenderCommand::Clear();

	// ----------- BEGIN SCENE -------------//
	Dank::Renderer2D::BeginScene(_cameraController.GetCamera());
	
	Dank::Renderer2D::DrawQuad({ 0.2f, -0.4f }, { 0.45f, 0.45f }, { 0.7f, 0.2f, 0.3f, 1.0f });
	Dank::Renderer2D::DrawQuad({ 1.2f, -0.4f }, { 0.45f, 0.45f }, { 0.7f, 0.2f, 0.3f, 1.0f });

	Dank::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, tex_weed, 5.0f);
	Dank::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, tex_smile, 5.0f);

	Dank::Renderer2D::DrawQuad({ 0.5f, 0.5f, }, { 1.0f, 1.0f }, tex_weed, 5.0f);
	Dank::Renderer2D::DrawQuad({ -0.5f, -0.5f, }, { 1.0f, 1.0f }, tex_smile, 5.0f);

	//Dank::Renderer2D::DrawRotatedQuad({ -0.75f, -0.1f, 0.3f }, { 0.4f, 0.75f }, glm::radians(10.0f), {0.2f, 0.3f, 0.7f, 1.0f});
	//Dank::Renderer2D::DrawRotatedQuad({ -0.75f, -0.5f, 0.3f }, { 0.4f, 0.75f }, 10.0f, { 0.8f, 0.8f, 0.8f, 1.0f });

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