#pragma once
#include "dankpch.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Dank {

	class Model
	{
	public:
		Model();


		const glm::mat4& GetModel() const { return _model; }
		const glm::mat4& UpdateModel(float runtime);

	private:
		glm::mat4 _model = glm::mat4(1.0f);
	};

}
