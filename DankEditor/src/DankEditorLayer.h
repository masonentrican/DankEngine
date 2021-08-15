#pragma once

#include "Dank.h"
#include "DankEngine/Core/Core.h"

#include "Panels/SceneHierarchyPanel.h"

namespace Dank
{

	class DankEditorLayer : public Layer {
	public:
		DankEditorLayer();
		virtual ~DankEditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;

		virtual void OnImGuiRender() override;

	private:
		OrthographicCameraController _cameraController;

		Ref<VertexArray> _vertexArray;
		Ref<Texture2D> tex_weed, tex_smile;
		Ref<Framebuffer> _framebuffer;

		ShaderLibrary _shaderLibrary;
		Ref<Shader> _2DShader;
		Ref<Shader> _3DShader;
		Ref<Dank::Model> ourModel;
		std::string ModelPath;
		glm::vec3 _shaderDefaultColor = { 1.0f, 1.0f, 1.0f };

		glm::vec2 _viewportSize = { 0, 0 };

		Ref<Scene> _activeScene;

		SceneHierarchyPanel _sceneHierarchyPanel;

		bool _primaryCamera = false;

		bool _viewportFocused = false;
		bool _viewportHovered = false;

		// UI toggles
		bool _ui_editor_style = false;
	};
}