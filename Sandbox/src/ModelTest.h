#pragma once
#include <dankpch.h>
#include <Dank.h>

	class ModelLayer : public Dank::Layer
	{
	public:
		ModelLayer();
		virtual ~ModelLayer() = default;
		void OnUpdate(Dank::Timestep ts) override;
	

	private:
		Dank::PerspectiveCameraController _cameraController;
		std::string ModelPath;
		Dank::ShaderLibrary _shaderLibrary;
		Dank::Ref<Dank::Shader> _defaultShader;
		Dank::Ref<Dank::VertexArray> _vertexArray;
		Dank::Ref<Dank::Model> ourModel;
	};

