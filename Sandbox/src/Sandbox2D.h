#pragma once

#include "Dank.h"
#include "DankEngine/Core/Core.h"

class Sandbox2D : public Dank::Layer{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	
	void OnUpdate(Dank::Timestep ts) override;
	void OnEvent(Dank::Event& e) override;
	
	virtual void OnImGuiRender() override;

private:
	Dank::OrthographicCameraController _cameraController;
	
	Dank::Ref<Dank::VertexArray> _vertexArray;
	Dank::Ref<Dank::Texture2D> _texture;

	Dank::ShaderLibrary _shaderLibrary;
	Dank::Ref<Dank::Shader> _defaultShader;

	glm::vec3 _shaderDefaultColor = { 1.0f, 1.0f, 1.0f };

	bool _isShaderSettingsOpen = true;
	
	Dank::Application& app = Dank::Application::Get();
};