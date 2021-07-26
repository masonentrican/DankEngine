#pragma once

#include "OrthographicCamera.h"
#include "DankEngine/Core/Timestep.h"

#include "DankEngine/Events/ApplicationEvent.h"
#include "DankEngine/Events/MouseEvent.h"

namespace Dank
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		
		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return _camera; }
		const OrthographicCamera& GetCamera() const { return _camera; }

		float GetZoomLevel() const { return _zoomLevel; }
		void SetZoomLevel(float level) { _zoomLevel = level; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float _aspectRatio;
		float _zoomLevel = 1.0f;
		OrthographicCamera _camera;

		bool _rotation;

		glm::vec3 _cameraPosition = { 0.0f, 0.0f, 0.0f };
		float _cameraRotation = 0.0f;

		float _cameraTranslationSpeed = 5.0f;
		float _cameraRotationSpeed = 180.0f;
	};
}