#include "DankEditorLayer.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

namespace Dank
{
	DankEditorLayer::DankEditorLayer() : Layer("DankEditorLayer"), _cameraController(60.0f, 1280.0f / 720.0f)
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
        //ModelPath = "assets/models/backpack/backpack.obj";
        ModelPath = "assets/models/bear_joined_decimated.fbx";
        auto defaultShader = _shaderLibrary.Load("assets/shaders/object.glsl");
        _objectShader = _shaderLibrary.Get("object");
        auto lightShader = _shaderLibrary.Load("assets/shaders/lightsource.glsl");
        _lightShader = _shaderLibrary.Get("lightsource");

        ourModel = std::make_shared<Dank::Model>(Dank::Model(ModelPath, _objectShader));
        cube = std::make_shared<Dank::Primitive>(Renderer::CreatePrimitive("Cube"));
        light = std::make_shared<LightSource>(LightSource(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)));

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
        Renderer::BeginScene(_cameraController.GetCamera());
        
        Dank::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Dank::RenderCommand::Clear();

        float lightX = 2.0f * sin(Dank::Application::Get().GetLastFrameTime());
        float lightY = 0.0f;
        float lightZ = 1.5f * cos(Dank::Application::Get().GetLastFrameTime());
        glm::vec3 lightPos = glm::vec3(lightX, lightY, lightZ);

        light->SetPosition(lightPos);
        // render the loaded model
         glm::mat4 model = glm::mat4(1.0f);
         model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
         model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_objectShader)->Bind();
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_objectShader)->UploadUniformMat4("model", model);
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_objectShader)->UploadUniformMat4("u_ViewProjection", _cameraController.GetCamera().GetViewProjectionMatrix());
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_objectShader)->UploadUniformFloat3("viewPos", _cameraController.GetPosition());
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_objectShader)->UploadUniformFloat3("lightPos", light->_position);
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_objectShader)->UploadUniformFloat3("light.ambient", light->_ambient);
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_objectShader)->UploadUniformFloat3("light.diffuse", light->_diffuse);
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_objectShader)->UploadUniformFloat3("light.specular", light->_specular);
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_objectShader)->Unbind();
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_lightShader)->Bind();
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_lightShader)->UploadUniformMat4("u_ViewProjection", _cameraController.GetCamera().GetViewProjectionMatrix());
         std::dynamic_pointer_cast<Dank::OpenGLShader>(_lightShader)->Unbind();
         Dank::Renderer::DrawModel(ourModel);
        glm::vec3 pos(2.0f, 0.0f, 2.0f);
        float size = 1.0f;
        glm::vec3 color(1.0f, 0.0f, 0.0f);
        
       Renderer::DrawPrimitive(cube->_VA, cube->_VB, pos, size, color, _objectShader);
        Renderer::DrawLightSource(light->_VA, light->_VB, light->_position, 0.2f, _lightShader);
        Dank::Renderer::EndScene();
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