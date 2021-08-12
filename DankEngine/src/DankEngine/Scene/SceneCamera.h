#pragma once

#include "DankEngine/Renderer/Camera.h"

namespace Dank
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetViewportSize(uint32_t width, uint32_t height);

		// Probably dont need these but fuck it
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFov, float nearClip, float farClip);

		// Perspective
		float GetPerspectiveFov()	   const { return _perspectiveFov; }
		float GetPerspectiveNearClip() const { return _perspectiveNearClip; }
		float GetPerspectiveFarClip()  const { return _perspectiveFarClip; }

		void  SetPerspectiveFov(float fov)             { _perspectiveFov = fov; RecalculateProjection(); }
		void  SetPerspectiveNearClip(float nearClip)   { _perspectiveNearClip = nearClip; RecalculateProjection(); }
		void  SetPerspectiveFarClip(float farClip)	   { _perspectiveFarClip = farClip; RecalculateProjection(); }

		// Orthographic
		float GetOrthographicSize()     const { return _orthographicSize; }
		float GetOrthographicNearClip() const { return _orthographicNearClip; }
		float GetOrthographicFarClip()  const { return _orthographicFarClip; }

		void SetOrthographicSize(float size)         { _orthographicSize = size; RecalculateProjection(); }
		void SetOrthographicNearClip(float nearClip) { _orthographicNearClip = nearClip; RecalculateProjection(); }
		void SetOrthographicFarClip(float farClip)   { _orthographicFarClip = farClip; RecalculateProjection(); }

		// Projection
		ProjectionType GetProjectionType() const { return _projectionType; }
		void SetProjectionType(ProjectionType type) { _projectionType = type; RecalculateProjection(); }



	private:
		void RecalculateProjection();

	private:
		ProjectionType _projectionType = ProjectionType::Orthographic;

		float _orthographicSize = 10.0f;
		float _orthographicNearClip = -1.0f;
		float _orthographicFarClip = 1.0f;		

		float _perspectiveFov = glm::radians(50.0f);
		float _perspectiveNearClip = 0.01f;
		float _perspectiveFarClip = 5000.0f;

		float _aspectRatio = 0.0f;
	};
}
