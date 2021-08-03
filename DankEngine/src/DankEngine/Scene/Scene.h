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

	private:
		entt::registry _registry;

		friend class Entity;
	};
}