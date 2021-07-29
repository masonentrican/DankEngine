#include <Dank.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "ImGui/ImGui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ModelTest.h"





	ModelLayer::ModelLayer() : Layer("Model"), _cameraController(45.0f, 1280.0f/720.0f)
	{
		ModelPath = "assets/models/backpack/backpack.obj";
		
		//Dank::Model testModel(ModelPath);
		auto defaultShader = _shaderLibrary.Load("assets/shaders/test.glsl");
		_vertexArray.reset(Dank::VertexArray::Create());
		ourModel = new Dank::Model(ModelPath);
		
	}

	void ModelLayer::OnUpdate(Dank::Timestep ts)
	{
		//Dank::Model testModel(ModelPath);
		Dank::Renderer::BeginScene(_cameraController.GetCamera());
		_cameraController.OnUpdate(ts.GetSeconds());
		Dank::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Dank::RenderCommand::Clear();
		
		_defaultShader = _shaderLibrary.Get("test");

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
		std::dynamic_pointer_cast<Dank::OpenGLShader>(_defaultShader)->Bind();
		std::dynamic_pointer_cast<Dank::OpenGLShader>(_defaultShader)->UploadUniformMat4("model", model);
		std::dynamic_pointer_cast<Dank::OpenGLShader>(_defaultShader)->UploadUniformMat4("u_ViewProjection", _cameraController.GetCamera().GetViewProjectionMatrix());

		ourModel->Draw(_defaultShader);

		Dank::Renderer::EndScene();

		
		
	}




