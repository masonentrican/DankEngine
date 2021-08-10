#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Dank {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			DANK_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			return _scene->_registry.emplace<T>(_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			DANK_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return _scene->_registry.get<T>(_entityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return _scene->_registry.has<T>(_entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			DANK_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			_scene->_registry.remove<T>(_entityHandle);
		}

		operator bool() const { return _entityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)_entityHandle; }

		// Is equal compare
		bool operator==(const Entity& other) const
		{
			return _entityHandle == other._entityHandle && _scene == other._scene;
		}

		// Not equal compare
		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity _entityHandle { 0 };
		Scene* _scene = nullptr;
	};

}