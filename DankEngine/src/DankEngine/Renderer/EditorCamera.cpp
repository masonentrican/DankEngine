#include "dankpch.h"
#include "EditorCamera.h"

namespace Dank
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: _fov(fov), _aspectRatio(aspectRatio), _nearClip(nearClip), _farClip(farClip)
	{
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
	}

	// Main camera view update loop
	void EditorCamera::UpdateView()
	{
		// Calculate position



		// Calculate orientation
		// Calculate view matrix
			// 
	}
}
