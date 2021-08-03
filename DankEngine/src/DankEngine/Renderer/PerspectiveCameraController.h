#pragma once

#include "PerspectiveCamera.h"
#include "DankEngine/Core/Timestep.h"

#include "DankEngine/Events/ApplicationEvent.h"
#include "DankEngine/Events/MouseEvent.h"

namespace Dank {
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float FOV = 45.0f, float aspectRatio = 1280.0f / 720.0f);
		PerspectiveCamera& GetCamera() { return _camera; }
		const PerspectiveCamera& GetCamera() const { return _camera; }
		void OnUpdate(float deltaTime);
		void OnEvent(Event& e);
		void OnResize(float width, float height);

	private:
		PerspectiveCamera _camera;
		float _aspectRatio;
		float _FOV;
		float _cameraSpeed = 0.05f;
		float _lastX;
		float _lastY;
		float _sensitivity = 0.05f;
		float _panSens = 0.03f;
		float _yaw = -90.0f;
		float _pitch = 0.0f;
		glm::vec3 _cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 _cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 _cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	};
}