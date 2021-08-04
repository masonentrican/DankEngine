#pragma once

#include "DankEngine/Renderer/Camera.h"

namespace Dank
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetOrthographicSize(float size) { _orthographicSize = size; RecalculateOrthographicProjection(); }

		void SetPerspective();

		void SetViewportSize(uint32_t width, uint32_t height);
		
		float GetOrthographicSize() const { return _orthographicSize; }

	private:
		void RecalculateOrthographicProjection();
		void RecalculatePerspectiveProjection();

	private:
		float _orthographicSize = 10.0f;
		float _orthographicNear = -1.0f;
		float _orthographicFar = 1.0f;		

		float _aspectRatio = 0.0f;
	};
}
