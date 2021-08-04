#include "dankpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dank
{
	SceneCamera::SceneCamera()
	{
		RecalculateOrthographicProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		_orthographicSize = size;
		_orthographicNear = nearClip;
		_orthographicFar  = farClip;
		RecalculateOrthographicProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		_aspectRatio = (float)width / (float)height;
		RecalculateOrthographicProjection();
	}

	void SceneCamera::RecalculateOrthographicProjection()
	{
		float orthoLeft    = -_orthographicSize * _aspectRatio * 0.5f;
		float orthoRight   =  _orthographicSize * _aspectRatio * 0.5f;
		float orthoBottom  = -_orthographicSize * 0.5f;
		float orthoTop     =  _orthographicSize * 0.5f;

		_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, _orthographicNear, _orthographicFar);
	}
}