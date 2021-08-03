#pragma once

#include <glm/glm.hpp>

namespace Dank {
	class Camera
	{
	public:
		Camera(const glm::mat4& projection) : _projection(projection) {}

		const glm::mat4& GetProjection() const { return _projection; }

	private:
		glm::mat4 _projection;
	};
}