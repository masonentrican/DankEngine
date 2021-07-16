#include "dankpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dank
{
	PerspectiveCamera::PerspectiveCamera()
	{
		
		_projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		_viewMatrix = glm::translate(_viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(0, 0, 1));

		_viewMatrix = glm::inverse(transform);
		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
		;
	}
}
