#pragma once

#include <glm/glm.hpp>

namespace Dank {
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const glm::vec3& getPosition() const { return _position; }
		float GetRotation() const { return _rotation; }

		void SetPosition(const glm::vec3& position) { _position = position; RecalculateViewMatrix(); }
		void SetRotation(const float rotation) { _rotation = rotation; RecalculateViewMatrix(); }

		void SetProjection(float left, float right, float bottom, float top);

		const glm::mat4& GetProjectionMatrix() const { return _projectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return _viewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return _viewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;
		glm::mat4 _viewProjectionMatrix;

		glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
		float _rotation = 0.0f;

	};
}