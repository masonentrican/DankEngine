#include "dankpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dank
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		_projectionType = ProjectionType::Orthographic;
		_orthographicSize = size;
		_orthographicNearClip = nearClip;
		_orthographicFarClip = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		_projectionType = ProjectionType::Perspective;
		_perspectiveFov = fov;
		_perspectiveNearClip = nearClip;
		_perspectiveFarClip = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		_aspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		switch (_projectionType)
		{
			case ProjectionType::Perspective:
				_projection = glm::perspective(_perspectiveFov, _aspectRatio, _perspectiveNearClip, _perspectiveFarClip);
				break;

			case ProjectionType::Orthographic:
				float orthoLeft = -_orthographicSize * _aspectRatio * 0.5f;
				float orthoRight = _orthographicSize * _aspectRatio * 0.5f;
				float orthoBottom = -_orthographicSize * 0.5f;
				float orthoTop = _orthographicSize * 0.5f;

				_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, _orthographicNearClip, _orthographicFarClip);
				break;
		}
	}
}