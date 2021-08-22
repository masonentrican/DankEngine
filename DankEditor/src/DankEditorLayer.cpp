#include "DankEditorLayer.h"
#include "ImGui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"

#include "DankEngine/NativeScripts/CameraController.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include "DankEngine/Scene/SceneSerializer.h"
#include "DankEngine/Utils/PlatformUtils.h"

#include "DankEngine/Math/Math.h"

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

        // Create the scene and set the hierarchy panel context.
        _activeScene = CreateRef<Scene>();
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
        if (_viewportFocused)
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
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(DANK_BIND_EVENT(DankEditorLayer::OnKeyPressed));
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
        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowMinSize.x = 350.0f; //TODO: Temporary hard 350pixel min width

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = 32.0f; //TODO: Back to imgui default min


        // Menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New","Ctrl+N"))
                    NewScene();

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    OpenScene();

                if (ImGui::MenuItem("Save As...","Ctrl+Shift+S"))
                    SaveSceneAs();

                ImGui::MenuItem("Style Editor", NULL, &_ui_editor_style);

                if (ImGui::MenuItem("Close")) Application::Get().Close();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }


        // Editor Style
        if (_ui_editor_style)
        {
	        ImGui::Begin("Editor Style Settings", &_ui_editor_style);
        	ImGui::ShowStyleEditor();
        	ImGui::End();
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
            Application::Get().GetImGuiLayer()->AllowEvents(_viewportFocused || _viewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            _viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

            // Send the frame buffer image to the viewport panel
            uint32_t frameBufferTextureId = _framebuffer->GetColorAttachmentRendererID();
            ImGui::Image((void*)frameBufferTextureId, ImVec2{ _viewportSize.x, _viewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });


            // Gizmos
            {
                Entity selectedEntity = _sceneHierarchyPanel.GetSelectedEntity();
                if (selectedEntity && _gizmoType != -1)
                {
                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();

                    ImGuizmo::SetRect(
                        ImGui::GetWindowPos().x,
                        ImGui::GetWindowPos().y,
                        ImGui::GetWindowWidth(),
                        ImGui::GetWindowHeight()
                    );

                    // Camera
                    // Need the projection and view
                    auto cameraEntity = _activeScene->GetPrimaryCameraEntity();

                    // Make sure we actually have a camera
                    if (cameraEntity)
                    {
                        auto& cameraComp = cameraEntity.GetComponent<CameraComponent>().Camera;
                        const glm::mat4 cameraProj = cameraComp.GetProjection();
                        glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

                        // Transform
                        // Need details of entity selected in scene hierarchy
                        auto& transformComp = selectedEntity.GetComponent<TransformComponent>();

                        // The transform we'll apply modifications to from maniuplating the imguizmo widget
                        glm::mat4 transform = transformComp.GetTransform();

                        // Draw the manipulate gizmo
                        ImGuizmo::Manipulate(
                            glm::value_ptr(cameraView),
                            glm::value_ptr(cameraProj),
                            (ImGuizmo::OPERATION)_gizmoType,
                            ImGuizmo::LOCAL,
                            glm::value_ptr(transform),
                            nullptr,
                            nullptr
                        );

                        // Actually manipulate the selected transform
                        if (ImGuizmo::IsUsing())
                        {
                            // We need to decompose the transform into position, rotation, and scale.
                            // glm has a library for this however it has bloat.
                            glm::vec3 position, rotation, scale;

                            Math::DecomposeTransform(transform, position, rotation, scale);

                            glm::vec3 deltaRotation = rotation - transformComp.Rotation;

                            transformComp.Rotation += deltaRotation;
                            transformComp.Position = position;
                            transformComp.Scale = scale;

                            
                            

                        }
                    }

                }
                
            }


            ImGui::End();
            ImGui::PopStyleVar();
        }


        ImGui::End();   // Docking End
    }

    bool DankEditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(DANK_KEY_LEFT_CONTROL) || Input::IsKeyPressed(DANK_KEY_RIGHT_CONTROL);
        bool shift = Input::IsKeyPressed(DANK_KEY_LEFT_SHIFT) || Input::IsKeyPressed(DANK_KEY_RIGHT_SHIFT);


        switch (e.GetKeyCode())
        {
        case DANK_KEY_N:
	    {
	        if (control)
                NewScene();
            break;
	    }

        case DANK_KEY_O:
        {
            if (control)
                OpenScene();
            break;
        }

        case DANK_KEY_S:
        {
            if (control && shift)
                SaveSceneAs();
        	break;
        }
        // Gizmo control
        case DANK_KEY_Q:
            _gizmoType = -1;
            break;

        case DANK_KEY_W:
            _gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;

        case DANK_KEY_E:
            _gizmoType = ImGuizmo::OPERATION::ROTATE;
            break;

        case DANK_KEY_R:
            _gizmoType = ImGuizmo::OPERATION::SCALE;
            break;

        default:
            break;
        }

	    
    }

    void DankEditorLayer::NewScene()
    {
        _activeScene = CreateRef<Scene>();
        _activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
        _sceneHierarchyPanel.SetContext(_activeScene);
    }

    void DankEditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Dank Engine Scene (*.dank)\0*.dank\0");
        if (!filepath.empty())
        {
            _activeScene = CreateRef<Scene>();
            _activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
            _sceneHierarchyPanel.SetContext(_activeScene);

            SceneSerializer serializer(_activeScene);
            serializer.DeserializeYAML(filepath);
        }
    }

    void DankEditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Dank Engine Scene (*.dank)\0*.dank\0");
        if (!filepath.empty())
        {
            SceneSerializer serializer(_activeScene);
            serializer.SerializeToYAML(filepath);
        }
    }
}
