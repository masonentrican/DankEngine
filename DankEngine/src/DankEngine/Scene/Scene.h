#pragma once

#include "entt.hpp"

#include "DankEngine/Core/Timestep.h"

namespace Dank
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry _registry;
		uint32_t _viewportWidth = 0, _viewportHeight = 0;

		friend class Entity;
	};
}