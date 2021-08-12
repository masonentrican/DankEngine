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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			_selectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if (_selectionContext)
			DrawComponents(_selectionContext);

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

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				// Camera Component Object and Camera Object refs
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);				

				// Gui text definitions
				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = { projectionTypeStrings[(int)camera.GetProjectionType()] };

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isCurrentProjType = currentProjectionTypeString == projectionTypeStrings[i];

						if (ImGui::Selectable(projectionTypeStrings[i], isCurrentProjType))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isCurrentProjType)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				switch (camera.GetProjectionType())
				{
					case SceneCamera::ProjectionType::Perspective:
					{
						float fov = glm::degrees(camera.GetPerspectiveFov());
						if (ImGui::DragFloat("Fov", &fov))
							camera.SetPerspectiveFov(glm::radians(fov));

						float persNear = camera.GetPerspectiveNearClip();
						if (ImGui::DragFloat("Near Clip", &persNear))
							camera.SetPerspectiveNearClip(persNear);

						float persFar = camera.GetPerspectiveFarClip();
						if (ImGui::DragFloat("Far Clip", &persFar))
							camera.SetPerspectiveFarClip(persFar);
					}
					break;

					case SceneCamera::ProjectionType::Orthographic:
					{
						float orthoSize = camera.GetOrthographicSize();
						if (ImGui::DragFloat("Size", &orthoSize))
							camera.SetOrthographicSize(orthoSize);

						float orthoNear = camera.GetOrthographicNearClip();
						if (ImGui::DragFloat("Near Clip", &orthoNear))
							camera.SetOrthographicNearClip(orthoNear);

						float orthoFar = camera.GetOrthographicFarClip();
						if (ImGui::DragFloat("Far Clip", &orthoFar))
							camera.SetOrthographicFarClip(orthoFar);
					}
					break;
				}				

				ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);

				ImGui::TreePop();
			}
		}
	}
}