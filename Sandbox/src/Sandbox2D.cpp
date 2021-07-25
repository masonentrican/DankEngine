#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

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

	static float rotation = 0.0f;
	static float yValue = 0.0f;

	rotation += ts * 50.0f;

	if (yValue < 1000.0f)
		yValue += ts * 50.0f;
	else
		yValue = 0.0f;

	// ----------- BEGIN SCENE -------------//
	Dank::Renderer2D::BeginScene(_cameraController.GetCamera());
	
	//Dank::Renderer2D::DrawQuad({ 0.2f, -0.4f }, { 0.45f, 0.45f }, { 0.2f, 0.7f, 0.3f, 1.0f });

	//Dank::Renderer2D::DrawQuad({ -0.2f, 0.4f }, { 0.45f, 0.45f }, tex_smile, 1.0f);

	//Dank::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f, }, { 1.0f, 1.0f }, 45.0f, { 0.2f, 0.3f, 0.7f, 1.0f });

	//Dank::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, }, { 1.0f, 1.0f }, rotation, tex_weed, 1.0f);

	for (int y = 0; y < 80; y++)
	{
		for (int x = 0; x < 80; x++)
		{
			if ((x ^ y) % 5 < 1)
				Dank::Renderer2D::DrawRotatedQuad({ (x * 0.125f) - (40.0f * 0.125f), (y * 0.125f - yValue / 400) - (40.0f * 0.125f) }, { 0.1f, 0.1f }, rotation, tex_smile, 1.0f);
			else
				Dank::Renderer2D::DrawRotatedQuad({ (x * 0.125f) - (40.0f * 0.125f), (y * 0.125f - yValue / 400) - (40.0f * 0.125f) }, { 0.1f, 0.1f }, rotation, tex_weed, 1.0f);
		}
	}

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