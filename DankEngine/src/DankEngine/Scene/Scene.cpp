#include "dankpch.h"
#include "Scene.h"

#include "Components.h"
#include "DankEngine/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Dank {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { _registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update scripts
		{
			_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nativeScriptComponent)
			{
				// TODO: Move to on scene play once it exists.
				if (!nativeScriptComponent.Instance)
				{
					nativeScriptComponent.Instance = nativeScriptComponent.Instantiate();
					nativeScriptComponent.Instance->_entity = Entity{ entity, this };
					nativeScriptComponent.Instance->OnCreate();
				}

				nativeScriptComponent.Instance->OnUpdate(ts);
			});
		}


		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		// Locate the main camera
		{
			DANK_PROFILE_SCOPE("Entt Camera Transform lookup");

			auto view = _registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}

			}
		}

		// If main camera found, begin rendering
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		_viewportWidth = width;
		_viewportHeight = height;

		// Resize any non-fixed aspect ratio camera
		auto view = _registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	

}