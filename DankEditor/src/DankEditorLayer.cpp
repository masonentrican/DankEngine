#include "DankEditorLayer.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

namespace Dank
{
	DankEditorLayer::DankEditorLayer() : Layer("DankEditorLayer"), _cameraController(1280.0f / 720.0f)
	{

	}

	void DankEditorLayer::OnAttach()
	{
		tex_weed = Dank::Texture2D::Create("assets/textures/weedleaf.png");
		tex_smile = Dank::Texture2D::Create("assets/textures/awesomeface.png");

		Dank::FramebufferSpecification frameBufferSpec;
		frameBufferSpec.Width = 1280;
		frameBufferSpec.Height = 720;

		_framebuffer = Dank::Framebuffer::Create(frameBufferSpec);

	}
	void DankEditorLayer::OnDetach()
	{
		DANK_PROFILE_FUNCTION();

	}

	void DankEditorLayer::OnUpdate(Dank::Timestep ts)
	{
		DANK_PROFILE_FUNCTION();

        // Resize viewport if need be
        if (FramebufferSpecification spec = _framebuffer->GetSpecification();
            _viewportSize.x > 0.0f && _viewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != _viewportSize.x || spec.Height != _viewportSize.y))
        {
            _framebuffer->Resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
            _cameraController.OnResize(_viewportSize.x, _viewportSize.y);
        }

        Dank::Renderer2D::ResetStats();

        if(_viewportFocused)
		    _cameraController.OnUpdate(ts);

		_framebuffer->Bind();

		Dank::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Dank::RenderCommand::Clear();

		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		// ----------- BEGIN SCENE -------------//
		Dank::Renderer2D::BeginScene(_cameraController.GetCamera());

		Dank::Renderer2D::DrawQuad({ 0.2f, -0.4f }, { 0.45f, 0.45f }, { 0.2f, 0.7f, 0.3f, 1.0f });

		Dank::Renderer2D::DrawQuad({ -0.2f, 0.4f }, { 0.45f, 0.45f }, tex_smile, 1.0f);

		Dank::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f, }, { 1.0f, 1.0f }, 45.0f, { 0.2f, 0.3f, 0.7f, 1.0f });

		Dank::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, }, { 1.0f, 1.0f }, rotation, tex_weed, 1.0f);

		Dank::Renderer2D::EndScene();
		// -----------  END SCENE  -------------//

		_framebuffer->Unbind();

	}

	void DankEditorLayer::OnEvent(Dank::Event& e)
	{
		_cameraController.OnEvent(e);
	}

    void DankEditorLayer::OnImGuiRender()
    {
        DANK_PROFILE_FUNCTION();

        // Docking code here found from imgui_demo.cpp under docking demo section

        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static bool p_open = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &p_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close")) Application::Get().Close();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Stats Window
        {
            DANK_PROFILE_SCOPE("Stats Window");

            ImGui::Begin("Stats");

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
            ImGui::End();
        }

        // Viewport
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
            ImGui::Begin("Viewport");            
            
            // Check if the viewport is focused and hovered so we can
            // tell the ImGui layer whether or not events should be
            // marked as handled once recieved.
            _viewportFocused = ImGui::IsWindowFocused(); 
            _viewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->AllowEvents(_viewportFocused && _viewportHovered);            

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();            
            _viewportSize = { viewportPanelSize.x, viewportPanelSize.y };         

            // Send the frame buffer image to the viewport panel
            uint32_t frameBufferTextureId = _framebuffer->GetColorAttachmentRendererID();
            ImGui::Image((void*)frameBufferTextureId, ImVec2{ _viewportSize.x, _viewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

            ImGui::End();
            ImGui::PopStyleVar();
        }
        

        ImGui::End();   // Docking End
	}
}