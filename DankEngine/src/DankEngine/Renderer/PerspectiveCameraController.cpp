#include "dankpch.h"
#include "PerspectiveCameraController.h"

#include "DankEngine/Input.h"
#include "DankEngine/KeyCodes.h"

namespace Dank {
	PerspectiveCameraController::PerspectiveCameraController(float FOV, float aspectRatio) : _camera(FOV, aspectRatio)
	{
		_FOV = FOV;
		_aspectRatio = aspectRatio;
		_lastX = Input::GetMouseX();
		_lastY = Input::GetMouseY();
		
	}

	void PerspectiveCameraController::OnUpdate(float deltaTime)
	{
		_cameraSpeed = 2.5f;
		_cameraSpeed = _cameraSpeed * deltaTime;
		//std::cout << _cameraSpeed<< std::endl;
		if (Input::IsKeyPressed(DANK_KEY_A))
			_cameraPosition -= glm::normalize(glm::cross(_cameraFront, _cameraUp)) * _cameraSpeed;
		if (Input::IsKeyPressed(DANK_KEY_D))
			_cameraPosition += glm::normalize(glm::cross(_cameraFront, _cameraUp)) * _cameraSpeed;
		if (Input::IsKeyPressed(DANK_KEY_W))
			_cameraPosition += _cameraSpeed * _cameraFront;
		if (Input::IsKeyPressed(DANK_KEY_S))
			_cameraPosition -= _cameraSpeed * _cameraFront;
		if (Input::IsMouseButtonPressed(1))
		{
			float currentX = Input::GetMouseX();
			float currentY = Input::GetMouseY();

			float xOffset = currentX - _lastX;
			float yOffset = _lastY - currentY;
			_lastX = currentX;
			_lastY = currentY;

			xOffset *= _panSens;
			yOffset *= _panSens;

			_cameraPosition += yOffset * _cameraUp;
			_cameraPosition += xOffset * glm::normalize(glm::cross(_cameraFront, _cameraUp));
		}
		else
		{
			float currentX = Input::GetMouseX();
			float currentY = Input::GetMouseY();

			float xOffset = currentX - _lastX;
			float yOffset = _lastY - currentY;
			_lastX = currentX;
			_lastY = currentY;

			xOffset *= _sensitivity;
			yOffset *= _sensitivity;

			_yaw += xOffset;
			_pitch += yOffset;

			if (_pitch > 89.0f)
				_pitch = 89.0f;

			if (_pitch < -89.0f)
				_pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
			direction.y = sin(glm::radians(_pitch));
			direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
			_camera.SetRotation(glm::normalize(direction));
			_cameraFront = glm::normalize(direction);
		}
		_camera.SetPosition(_cameraPosition);
		//_camera.SetRotation(glm::normalize(direction));
		//_cameraFront = glm::normalize(direction);
		
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{

	}
}