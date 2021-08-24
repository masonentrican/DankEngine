#pragma once
#include "Camera.h"
#include "DankEngine/Core/Timestep.h"
#include "DankEngine/Events/Event.h"

namespace Dank
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

	private:
		void UpdateView();

	private:
		glm::mat4 _viewMatrix;

		float _fov				= 45.0f;
		float _aspectRatio		= 1.778f;
		float _nearClip			= 0.1f;
		float _farClip			= 5000.0f;

		float _viewportWidth	= 1280.0f;
		float _viewportHeight	= 720.0f;

		glm::vec3 _position		= { 0.0f, 0.0f, 0.0f };
		glm::vec3 _focalPoint   = { 0.0f, 0.0f, 0.0f };
		
	};
}
