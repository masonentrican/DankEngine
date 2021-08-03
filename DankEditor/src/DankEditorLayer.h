#pragma once

#include "Dank.h"
#include "DankEngine/Core/Core.h"

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
		PerspectiveCameraController _cameraController;

		Ref<VertexArray> _vertexArray;
		Ref<Texture2D> tex_weed, tex_smile;
		Ref<Framebuffer> _framebuffer;

		ShaderLibrary _shaderLibrary;
		Ref<Shader> _objectShader;
		Ref<Shader> _lightShader;
		Ref<Dank::Model> ourModel;
		Ref<Primitive> cube;
		std::string ModelPath;
		glm::vec3 _shaderDefaultColor = { 1.0f, 1.0f, 1.0f };

		glm::vec2 _viewportSize = { 0, 0 };

		bool _viewportFocused = false;
		bool _viewportHovered = false;
	};
}