#pragma once

#include <glm/glm.hpp>

namespace Dank {
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float FOV, float aspectRatio);

		const glm::vec3& getPosition() const { return _position; }
		float GetRotation() const { return _rotation; }

		void SetPosition(const glm::vec3& position) { _position = position; RecalculateViewMatrix(); }
		void SetRotation(const glm::vec3& newFront) { _front = newFront; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return _projectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return _viewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return _viewProjectionMatrix; }
		

	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix = glm::mat4(1.0f);
		glm::mat4 _viewProjectionMatrix;
		

		glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 _up =       glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 _front =    glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 _camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 _camDirection = glm::normalize(_position - _camTarget);
		glm::vec3 _camRight = glm::normalize(glm::cross(_up, _camDirection));
		glm::vec3 _camUp = glm::cross(_camDirection, _camRight);

		float _rotation = 0.0f;

	};
}
