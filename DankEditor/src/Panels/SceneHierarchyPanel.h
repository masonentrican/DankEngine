#pragma once

#include "DankEngine/Core/Core.h"
#include "DankEngine/Core/Log.h"

#include "DankEngine/Scene/Scene.h"
#include "DankEngine/Scene/Entity.h"

namespace Dank
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);

	private:
		Ref<Scene> _context;
		Entity _selectionContext;
	};
}
