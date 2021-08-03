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

	Dank::FramebufferSpecification frameBufferSpec;
	frameBufferSpec.Width = 1280;
	frameBufferSpec.Height = 720;
	
	_framebuffer = Dank::Framebuffer::Create(frameBufferSpec);

}
void Sandbox2D::OnDetach()
{
	DANK_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Dank::Timestep ts)
{
	DANK_PROFILE_FUNCTION();

	_cameraController.OnUpdate(ts);

	//DANK_TRACE("FPS: {0}", 1000 / ts.GetMilliseconds());

	Dank::Renderer2D::ResetStats();

	_framebuffer->Bind();

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
	//Dank::Renderer2D::BeginScene(_cameraController.GetCamera());
	
	//Dank::Renderer2D::DrawQuad({ 0.2f, -0.4f }, { 0.45f, 0.45f }, { 0.2f, 0.7f, 0.3f, 1.0f });

	//Dank::Renderer2D::DrawQuad({ -0.2f, 0.4f }, { 0.45f, 0.45f }, tex_smile, 1.0f);

	//Dank::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f, }, { 1.0f, 1.0f }, 45.0f, { 0.2f, 0.3f, 0.7f, 1.0f });

	//Dank::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, }, { 1.0f, 1.0f }, rotation, tex_weed, 1.0f);

	float scale = 0.5f;
	int res = 20;

	for (int y = 0; y < res; y++)
	{
		for (int x = 0; x < res; x++)
		{
			

			
			if ((x ^ y) % ((int)Dank::Application::Get().GetRunTime() + 1) < 1)
				Dank::Renderer2D::DrawRotatedQuad({ (x * scale) - (res / 2 * scale), (y * scale - yValue / 1000) - (res / 2 * scale) }, { scale * 0.9f, scale * 0.9f }, rotation, tex_smile, 1.0f);
			else
				Dank::Renderer2D::DrawRotatedQuad({ (x * scale) - (res / 2 * scale), (y * scale - yValue / 1000) - (res / 2 * scale) }, { scale * 0.9f, scale * 0.9f }, rotation, tex_weed, 1.0f);
		}
	}

	Dank::Renderer2D::EndScene();
	// -----------  END SCENE  -------------//

	_framebuffer->Unbind();

}

void Sandbox2D::OnEvent(Dank::Event& e)
{
	_cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	DANK_PROFILE_FUNCTION();
	ImGui::Begin("Settings");

	auto stats = Dank::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::NewLine();
	
	ImGui::Text("Application Stats:");
	ImGui::Text("FPS: %f", Dank::Application::Get().GetFps());
	ImGui::Text("Frame Count: %d", Dank::Application::Get().GetFrameCount());
	ImGui::Text("Runtime: %f", Dank::Application::Get().GetRunTime());
	ImGui::NewLine();

	uint32_t frameBufferTextureId = _framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)frameBufferTextureId, ImVec2{ 1280, 720 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	ImGui::End();
}