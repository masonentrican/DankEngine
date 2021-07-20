#include "dankpch.h"
#include "PerspectiveCamera.h"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Dank
{
	PerspectiveCamera::PerspectiveCamera(float FOV, float aspectRatio)
	{
																			
		_projectionMatrix = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
		_viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
								  glm::vec3(0.0f, 0.0f, -1.0f),
								  glm::vec3(0.0f, 1.0f, 0.0f));
		
		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
	
		
	}
	

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		_viewMatrix = glm::lookAt(_position, _position + _front, _up);
		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
		
	}
}
