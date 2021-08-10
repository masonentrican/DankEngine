#include "SceneHierarchyPanel.h"

#include <ImGui/imgui.h>

#include "DankEngine/Scene/Components.h"

namespace Dank
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		_context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		_context->_registry.each([&](auto entityID)
		{
			Entity entity{ entityID , _context.get() };
			DrawEntityNode(entity);
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		
		// Cast the entity id into a 64bit pointer for imgui to use as a unique id
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str()); 
		if (ImGui::IsItemClicked())
		{
			_selectionContext = entity;
		}

		// TODO: This is a test of child entities. Currently unsupported, using a random pointer id
		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)654654, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

	}
}