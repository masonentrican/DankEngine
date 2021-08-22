#include "dankpch.h"
#include "Scene.h"

#include "Components.h"
#include "DankEngine/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Dank {

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { _registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		_registry.destroy(entity);
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
		glm::mat4 cameraTransform;

		// Locate the main camera
		{
			DANK_PROFILE_SCOPE("Entt Camera Transform lookup");

			auto const view = _registry.view<TransformComponent, CameraComponent>();
			for (auto const entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}

			}
		}

		// If main camera found, begin rendering
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);

			const auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (const auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		_viewportWidth = width;
		_viewportHeight = height;

		// Resize any non-fixed aspect ratio camera
		const auto view = _registry.view<CameraComponent>();
		for (const auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		const auto view = _registry.view<CameraComponent>();
		for (const auto entity : view)
		{
			auto& cc = view.get<CameraComponent>(entity);
			if (cc.Primary)
				return Entity{entity, this};
		}
		return {};
	}

	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(_viewportWidth, _viewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	

}