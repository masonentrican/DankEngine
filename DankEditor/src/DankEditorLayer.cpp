#include "DankEditorLayer.h"
#include "ImGui/imgui.h"

#include "DankEngine/NativeScripts/CameraController.h"

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
		tex_weed = Texture2D::Create("assets/textures/weedleaf.png");
		tex_smile = Texture2D::Create("assets/textures/awesomeface.png");

        auto shader3D = _shaderLibrary.Load("assets/shaders/test.glsl");
        _3DShader = _shaderLibrary.Get("test");

        //ModelPath = "assets/models/backpack/backpack.obj";
        ModelPath = "assets/models/bear_joined_decimated.fbx";

        ourModel = CreateRef<Model>(Model(ModelPath, _3DShader));

		FramebufferSpecification frameBufferSpec;
		frameBufferSpec.Width = 1280;
		frameBufferSpec.Height = 720;

		_framebuffer = Framebuffer::Create(frameBufferSpec);



        // Entity Testing
        _activeScene = CreateRef<Scene>();

        auto greenSquare = _activeScene->CreateEntity("Green Square");
        greenSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 0.5f });

        auto redSquare = _activeScene->CreateEntity("Red Square");
        redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });        
        redSquare.GetComponent<TransformComponent>().Position = glm::vec3(0.5f, 0.0f, -0.1f);

        auto cameraEntity = _activeScene->CreateEntity("Camera 1");
        cameraEntity.AddComponent<CameraComponent>();        
        cameraEntity.GetComponent<TransformComponent>().Position = glm::vec3(0.0f, 0.0f, 5.0f);
        
        auto cameraEntity2 = _activeScene->CreateEntity("Camera 2");
        cameraEntity2.AddComponent<CameraComponent>().Primary = false;        

        cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        cameraEntity2.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        // Setup Scene Hierarchy Panel
        _sceneHierarchyPanel.SetContext(_activeScene);


	}
	void DankEditorLayer::OnDetach()
	{
		DANK_PROFILE_FUNCTION();
	}

	void DankEditorLayer::OnUpdate(Timestep ts)
	{
		DANK_PROFILE_FUNCTION();

        // Resize viewport if need be
        if (FramebufferSpecification spec = _framebuffer->GetSpecification();
            _viewportSize.x > 0.0f && _viewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != _viewportSize.x || spec.Height != _viewportSize.y))
        {
            _framebuffer->Resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
            _cameraController.OnResize(_viewportSize.x, _viewportSize.y);

            _activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
        }


        // Reset stats
        Renderer2D::ResetStats();


        // Update cam controller if focused
        if(_viewportFocused)
		    _cameraController.OnUpdate(ts);


        // Bind the frame buffer
		_framebuffer->Bind();

        // Clear it
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

        // Update the scene
        _activeScene->OnUpdate(ts);

        // Unbind the frame buffer
        _framebuffer->Unbind();

	}

	void DankEditorLayer::OnEvent(Event& e)
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
            DANK_PROFILE_SCOPE("Stats");

            ImGui::Begin("Stats");

            auto stats = Renderer2D::GetStats();
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
            ImGui::NewLine();

            ImGui::Text("Application Stats:");
            ImGui::Text("FPS: %f", Application::Get().GetFps());
            ImGui::Text("Frame Count: %d", Application::Get().GetFrameCount());
            ImGui::Text("Runtime: %f", Application::Get().GetRunTime());
            ImGui::NewLine();        

            ImGui::End();
        }

        // Scene Hierarchy Panel
        {
            DANK_PROFILE_SCOPE("Scene Hierarchy Panel");

            _sceneHierarchyPanel.OnImGuiRender();
        }

        // Viewport
        {
            DANK_PROFILE_SCOPE("Viewport");

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