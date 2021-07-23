#include "dankpch.h"
#include "Model.h"
#include "GLFW/glfw3.h"

namespace Dank {

	Model::Model()
	{
		_model = glm::rotate(_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	}

	const glm::mat4& Model::UpdateModel(float runtime)
	{
		_model = glm::rotate(_model, runtime * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		return _model;
	}
}

