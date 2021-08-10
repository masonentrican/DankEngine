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

        auto square = _activeScene->CreateEntity("Square from an entity");
        square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
        _squareEntity = square;

        _cameraEntity = _activeScene->CreateEntity("Camera Entity");
        _cameraEntity.AddComponent<CameraComponent>();
        
        _cameraEntity2 = _activeScene->CreateEntity("Clip-space cam");
        auto& cc = _cameraEntity2.AddComponent<CameraComponent>();
        cc.Primary = false;
        

        class CameraController : public ScriptableEntity
        {
        public:
            void OnCreate()
            {                
            }

            void OnDestroy()
            {
            }
            
            void OnUpdate(Timestep ts)
            {
                auto& transform = GetComponent<TransformComponent>().Transform;
                float speed = 5.0f;

                if (Input::IsKeyPressed(DANK_KEY_A))
                    transform[3][0] -= speed * ts;
                if (Input::IsKeyPressed(DANK_KEY_D))
                    transform[3][0] += speed * ts;
                if (Input::IsKeyPressed(DANK_KEY_W))
                    transform[3][1] += speed * ts;
                if (Input::IsKeyPressed(DANK_KEY_S))
                    transform[3][1] -= speed * ts;
            }
            
        };

        _cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        _cameraEntity2.AddComponent<NativeScriptComponent>().Bind<CameraController>();


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
            DANK_PROFILE_SCOPE("Stats Window");

            ImGui::Begin("Stats");

            auto stats = Renderer2D::GetStats();
            ImGui::Text("Renderer2D Stats:");
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

            // Debug out square entity stuff
            if (_squareEntity)
            {
                ImGui::Separator();
                auto& tag = _squareEntity.GetComponent<TagComponent>().Tag;
                ImGui::Text("%s", tag.c_str());

                auto& squareColor = _squareEntity.GetComponent<SpriteRendererComponent>().Color;
                ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
                ImGui::Separator();
            }

            // Point to the last column of the mat4 transform matrix.
            ImGui::DragFloat3("Camera Transform",
                glm::value_ptr(_cameraEntity.GetComponent<TransformComponent>().Transform[3]));

            ImGui::DragFloat3("Near Clip Camera Transform",
                glm::value_ptr(_cameraEntity2.GetComponent<TransformComponent>().Transform[3]));

            if (ImGui::Checkbox("Swap to Second Camera", &_primaryCamera))
            {
                _cameraEntity.GetComponent<CameraComponent>().Primary = !_primaryCamera;
                _cameraEntity2.GetComponent<CameraComponent>().Primary = _primaryCamera;
            }

            auto& camera = _cameraEntity2.GetComponent<CameraComponent>().Camera;
            float orthoSize = camera.GetOrthographicSize();
            if (ImGui::DragFloat("Second camera ortho size", &orthoSize))
                camera.SetOrthographicSize(orthoSize);

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