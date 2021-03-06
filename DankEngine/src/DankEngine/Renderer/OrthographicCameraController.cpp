#include "dankpch.h"
#include "OrthographicCameraController.h"

#include "DankEngine/Core/Input.h"
#include "DankEngine/Core/KeyCodes.h"

namespace Dank
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: _aspectRatio(aspectRatio), _camera(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel), _rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(DANK_KEY_A))
			_cameraPosition.x -= _cameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(DANK_KEY_D))
			_cameraPosition.x += _cameraTranslationSpeed * ts;
		
		if (Input::IsKeyPressed(DANK_KEY_W))
			_cameraPosition.y += _cameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(DANK_KEY_S))
			_cameraPosition.y -= _cameraTranslationSpeed * ts;

		if (_rotation)
		{
			if (Input::IsKeyPressed(DANK_KEY_Q))
				_cameraRotation += _cameraRotationSpeed * ts;
			else if (Dank::Input::IsKeyPressed(DANK_KEY_E))
				_cameraRotation -= _cameraRotationSpeed * ts;

			_camera.SetRotation(_cameraRotation);
		}

		_camera.SetPosition(_cameraPosition);

		// Camera move speed is relative to how zoomed we are
		_cameraTranslationSpeed = _zoomLevel;
		
		
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(DANK_BIND_EVENT(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(DANK_BIND_EVENT(OrthographicCameraController::OnWindowResized));
	}

	// Changes zoom level
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		_zoomLevel -= e.GetYOffset() * 0.1f;
		_zoomLevel = std::max(_zoomLevel, 0.25f);
		_camera.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);

		return false;
	}

	// Changes aspect ratio
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		_aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		_camera.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		
		return false;
	}
}